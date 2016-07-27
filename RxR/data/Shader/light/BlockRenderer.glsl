// helper class to render materials

#include "../uniforms/Lights.glsl"

uniform sampler3D mapTexVol;
#define SHADOW_STEP 0.5
#define SHADOW_TRESHOLD 0.0625

float getMapVolumeValue(vec3 pos)
{
	return texture3D(mapTexVol, vec3((pos.x + 0.5) / 50.0, (pos.y + 0.5) / 35.0, (pos.z + 0.5) / 4.0)).r;
}
bool isInMap(vec3 pos)
{
	return pos.x >= -0.5 && pos.y >= -0.5 && pos.z >= -0.5 	
			&& pos.x <= 50.5 && pos.y <= 35.5 && pos.z <= 4.5;
}

float getSoftShadowPointLight(vec3 start, vec3 dest)
{
	float f = 1.0;
	
	vec3 vstep = dest - start;
	if(vstep == vec3(0.0))
		return f;
	
	float pathLen = length(vstep);
	float curDist = SHADOW_STEP;
	vstep = normalize(vstep) * SHADOW_STEP;
	vec3 pos = start + vstep;
	
	while(curDist < pathLen )//&& isInMap(pos))
	{
		float v = (1.0 - getMapVolumeValue(pos));
		f *= v ;//* v;
		if(f < SHADOW_TRESHOLD)
			return 0.0;
		
		pos += vstep;
		curDist += SHADOW_STEP;
	}
	return f;
}


// using sampler2D
vec3 renderMapBlock(vec3 pos, vec3 normal, vec3 mdiff, vec3 mspec, float ngloss)
{
	vec3 color = mdiff * LightsAmbient;
	
	vec3 eyeDir = normalize(LightsEye - pos);
	// go through each source
	
	for(uint i = uint(0); i < LightsNLights; i++)
	{
		if(LightsLight[i].type == uint(0)) // directional
		{
			// lambert term
			vec3 reflectedLight = reflect(LightsLight[i].origin, normal);
			float theta = dot(reflectedLight , normal);
			if(theta < 0.0)
				continue; // light comes from other direction
			
			float phi = dot(reflectedLight, eyeDir);
			
			phi = max(0.0, phi);
			
			phi = pow(phi,ngloss);
			
			color += mdiff * LightsLight[i].color * theta;
			color += mspec * LightsLight[i].color * phi;
		}
		else // pointLight
		{
			float shadowFac = getSoftShadowPointLight(pos, LightsLight[i].origin);
			//return vec3(getMapVolumeValue(pos));
			//color += vec3(shadowFac);
			//continue;
			//shadowFac = 1.0;
			if(shadowFac < 0.001)
				continue;
			
			vec3 lightVec = pos - LightsLight[i].origin;//LightsLight[i].origin - pos;
			
			// check normal
			vec3 reflectedLight = reflect(normalize(lightVec) , normal);
			float theta = dot(reflectedLight, normal);
			if(theta < 0.0)
				continue;
			
			// check distance
			float invDistance = length(lightVec);
			invDistance *= invDistance;
			if(invDistance > 0.0) // only invert if distance != 0
				invDistance = 1.0 / invDistance;
			
			float factor = 1.0 / LightsLight[i].attenuation * invDistance;
			// if factor to small discard
			if(factor < 0.015)
				continue;
				//return vec3(0.0,0.0,1.0);
			factor = min(factor,1.0) * shadowFac;
			
				
			float phi = dot(reflectedLight, eyeDir);
			phi = max(0.0, phi);
			
			phi = pow(phi,ngloss);
			
			color += mdiff * LightsLight[i].color * theta * factor;
			color += mspec * LightsLight[i].color * phi * factor;
		}
	}

	return clamp(color,0.0,1.0);
}
