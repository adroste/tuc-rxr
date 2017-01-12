// helper class to render materials

#include "../uniforms/Lights.glsl"
#include "../uniforms/MapInfo.glsl"

// plz bind this to 0
uniform sampler3D mapTexVol;
#define SHADOW_STEP 0.5
#define SHADOW_TRESHOLD 0.0625
#define FACTOR_DISCARD 0.000024
#define FASTER_SHADOWS

const float SH_FUNC_M = 1.0 / (1.0 - SHADOW_TRESHOLD * SHADOW_TRESHOLD);
const float SH_FUNC_B = 1.0 - SH_FUNC_M;

float getMapVolumeValue(vec3 pos)
{
	return texture(mapTexVol, vec3((pos.x + 0.5) / MapDim.x, (pos.y + 0.5) / MapDim.y, (pos.z + 0.5) / MapDim.z)).r;
}
// pos := (pos + 0.5) / MapDim
float getMapVolumeValueRaw(vec3 pos)
{
	return texture(mapTexVol, pos).r;
}
bool isInMap(vec3 pos)
{
	return pos.x >= -0.5 && pos.y >= -0.5 && pos.z >= -0.5 	
			&& pos.x <= MapDim.x - 0.5 && pos.y <= MapDim.y - 0.5 && pos.z <= MapDim.z - 0.5;
}
// pos := (pos += 0.5) / MapDim
bool isInMapRaw(vec3 pos)
{
	return pos.x >= 0.0 && pos.y >= 0.0 && pos.z >= 0.0 	
			&& pos.x <= 1.0 && pos.y <= 1.0 && pos.z <= 1.0;
}
float smoothShadowValue(float x)
{
	// [SHADOW_TRESHOLD,1.0] -> [0.0,1.0] to smooth borders
	return SH_FUNC_M * x * x + SH_FUNC_B;
}

float getSoftShadowPointLight(vec3 start, vec3 dest, float prefac)
{
	float f = 1.0;
	
	vec3 vstep = dest - start;
	if(vstep == vec3(0.0))
		return f;
	
	float pathLen = length(vstep);
	float curDist = SHADOW_STEP;
	vstep = normalize(vstep) * SHADOW_STEP;
	vec3 pos = start + vstep;
	pos += vec3(0.5); // offset
	pos /= MapDim.xyz;
	vstep /= MapDim.xyz;
	
	bool leaped = false;
	while(curDist < pathLen && isInMapRaw(pos))
	{
		float v = getMapVolumeValueRaw(pos);
		f *= (1.0 - v);
		if(smoothShadowValue(f) * prefac < FACTOR_DISCARD)
			return 0.0;
		
#ifdef FASTER_SHADOWS

		if(leaped && v > 0.01)
		{
			// leaped in the wrong place..
			f *= (1.0 - getMapVolumeValueRaw(pos - vstep));
			f *= (1.0 - getMapVolumeValueRaw(pos - vstep * 2.0));

			if(f < SHADOW_TRESHOLD)
				return 0.0;
		}
		leaped = false;
		// leap?
		if(v < 0.01)
		{
			pos += vstep * 3.0;
			curDist += SHADOW_STEP * 3.0;
			leaped = true;
			continue;
		}
#endif
		pos += vstep;
		curDist += SHADOW_STEP;
	}
	return smoothShadowValue(f) * prefac;
}

float getSoftShadowDirectional(vec3 start, vec3 destOut)
{
	float f = 1.0;

	vec3 vstep = destOut * SHADOW_STEP;
	vec3 pos = start + vstep;
	pos += vec3(0.5);
	pos /= MapDim.xyz;
	vstep /= MapDim.xyz;
	
	bool leaped = false;
	while(isInMapRaw(pos))
	{
		float v = getMapVolumeValueRaw(pos);
		f *= (1.0 - v);
		if(f < SHADOW_TRESHOLD)
			return 0.0;

#ifdef FASTER_SHADOWS
		if(leaped && (v > 0.01))
		{
			// leaped in the wrong place..
			f *= (1.0 - getMapVolumeValueRaw(pos - vstep));
			f *= (1.0 - getMapVolumeValueRaw(pos - vstep * 2.0));
			if(f < SHADOW_TRESHOLD)
				return 0.0;
		}
		leaped = false;
		// big step?
		if(v < 0.01)
		{
			// step okay?
			
			pos += vstep * 3.0;
			leaped = true;
			continue;
		}
#endif
		pos += vstep;
	}
	return smoothShadowValue(f);
}

// using sampler2D
vec3 renderMapBlock(vec3 pos, vec3 normal, vec3 mdiff, vec3 mspec, float ngloss)
{
	vec3 color = mdiff * LightsAmbient;
	//color = vec3(0.0);
	
	vec3 eyeDir = normalize(LightsEye - pos);
	bool isSpecular = dot(mspec,mspec) > 0.0;
	// go through each source
	
	float theta = 0.0;
	float phi = 0.0;
	float factor = 0.0;
	vec3 reflectedLight = vec3(0.0);
	float lightLuma = 1.0;
	const vec3 lumaConst = vec3(0.299,0.587,0.114);
	
	for(uint i = uint(0); i < LightsNLights; i++)
	{
		lightLuma = dot(lumaConst,LightsLight[i].color);
		if(LightsLight[i].type == uint(0)) // directional
		{
			// lambert term
			reflectedLight = reflect(LightsLight[i].origin, normal);
			theta = dot(reflectedLight, normal);
			if(theta < 0.0)
				continue; // light comes from other direction
				
			factor = getSoftShadowDirectional(pos, -LightsLight[i].origin);
			if(factor * lightLuma <= FACTOR_DISCARD)
				continue;
		}
		else // pointLight
		{
			vec3 lightVec = pos - LightsLight[i].origin;
			
			// check normal
			float dist =  length(lightVec);
			reflectedLight = reflect(lightVec / dist , normal);
			theta = dot(reflectedLight, normal);
			if(theta < 0.0)
				continue;
			
			//factor = 1.0 / (1.0 + (LightsLight[i].attenuation * dist) *(1.0 + dist));
			float r = LightsLight[i].attenuation;
			float rinv = dist - r;
			factor = - (rinv * rinv * rinv) / (r * r * r);
			// if factor to small discard
			if(factor  <= FACTOR_DISCARD)
				continue;
				
			factor = getSoftShadowPointLight(pos, LightsLight[i].origin, factor);
			if(factor * lightLuma <= FACTOR_DISCARD)
				continue;
				//if(shadowFac > -0.5) return vec3(0.0,1.0,0.0);
				//else return vec3(0.0,0.0,1.0);
		}
		
		color += mdiff * LightsLight[i].color * theta * factor;
		if(isSpecular)
		{
			phi = dot(reflectedLight, eyeDir);
			phi = max(0.0, phi);
			phi = pow(phi,ngloss);
			color += mspec * LightsLight[i].color * phi * factor;
		}
	}

	return clamp(color,0.0,1.0);
}
