// helper class to render materials

#include "../uniforms/Lights.glsl"

// using sampler2D
vec3 renderMapBlock(vec3 pos, vec3 normal, vec3 mdiff, vec3 mspec, float ngloss)
{
	vec3 color = mdiff * LightsAmbient;
	
	vec3 eyeDir = normalize(LightsEye - pos);
	// go through each source
	
	for(uint i = 0; i < LightsNLights; i++)
	{
		if(LightsLight[i].type == 0) // directional
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
			factor = min(factor,1.0);
			
				
			float phi = dot(reflectedLight, eyeDir);
			phi = max(0.0, phi);
			
			phi = pow(phi,ngloss);
			
			color += mdiff * LightsLight[i].color * theta * factor;
			color += mspec * LightsLight[i].color * phi * factor;
		}
	}

	return clamp(color,0.0,1.0);
}
