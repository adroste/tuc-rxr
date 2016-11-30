// helper class to render materials

#include "../uniforms/Lights.glsl"
#include "../uniforms/MapInfo.glsl"

layout(binding = 0) uniform sampler3D mapTexVol;
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
bool isInMap(vec3 pos)
{
	return pos.x >= -0.5 && pos.y >= -0.5 && pos.z >= -0.5 	
			&& pos.x <= MapDim.x - 0.5 && pos.y <= MapDim.y - 0.5 && pos.z <= MapDim.z - 0.5;
}

float smoothShadowValue(float x)
{
	// [SHADOW_TRESHOLD,1.0] -> [0.0,1.0] to smooth borders
	float y = SH_FUNC_M * x * x + SH_FUNC_B;
	return y;
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
	
	bool leaped = false;
	while(curDist < pathLen && isInMap(pos))
	{
		float v = getMapVolumeValue(pos);
		f *= (1.0 - v);
		if(smoothShadowValue(f) * prefac < FACTOR_DISCARD)
			return 0.0;
		
#ifdef FASTER_SHADOWS

		if(leaped && v > 0.01)
		{
			// leaped in the wrong place..
			f *= (1.0 - getMapVolumeValue(pos - vstep));
			f *= (1.0 - getMapVolumeValue(pos - vstep * 2.0));
			if(smoothShadowValue(f) * prefac < FACTOR_DISCARD)
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
	
	bool leaped = false;
	while(isInMap(pos))
	{
		float v = getMapVolumeValue(pos);
		f *= (1.0 - v);
		if(f < SHADOW_TRESHOLD)
			return 0.0;

#ifdef FASTER_SHADOWS
		if(leaped && (v > 0.01))
		{
			// leaped in the wrong place..
			f *= (1.0 - getMapVolumeValue(pos - vstep));
			f *= (1.0 - getMapVolumeValue(pos - vstep * 2.0));
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
	
	vec3 eyeDir = normalize(LightsEye - pos);
	bool isSpecular = dot(mspec,mspec) > 0.0;
	// go through each source
	
	float theta = 0.0;
	float phi = 0.0;
	float factor = 0.0;
	vec3 reflectedLight = vec3(0.0);
	const vec3 lumaConst = vec3(0.299,0.587,0.114);
	
	for(uint i = uint(0); i < LightsNLights; i++)
	{
		if(LightsLight[i].type == uint(0)) // directional
		{
			// lambert term
			reflectedLight = reflect(LightsLight[i].origin, normal);
			theta = dot(reflectedLight, normal);
			if(theta < 0.0)
				continue; // light comes from other direction
				
			factor = getSoftShadowDirectional(pos, -LightsLight[i].origin);
			if(factor <= FACTOR_DISCARD)
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
			factor = - pow(dist - r,3) / (r * r * r);
			// if factor to small discard
			if(factor  <= FACTOR_DISCARD)
				//continue;
				return vec3(1.0,0.0,0.0);
				
			float shadowFac = getSoftShadowPointLight(pos, LightsLight[i].origin, factor);
			if(shadowFac <= FACTOR_DISCARD)
				continue;
				//return vec3(0.0,1.0,0.0);
			
			factor = /*min(factor,1.0) */ shadowFac;
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
