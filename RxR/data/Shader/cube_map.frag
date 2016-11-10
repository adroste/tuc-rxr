#version 330 core

#include "light/BlockRenderer.glsl"
#include "uniforms/Material.glsl"
#include "light/gamma.glsl"
#include "light/bump.glsl"

in vec3 mapPos;
flat in vec3 out_normal;
flat in vec3 diffColor;
flat in vec4 specColor;
flat in uint shaderType;
flat in uint plsDiscard;
flat in uint cubeSide;
flat in uint cubeNeighbors;
flat in vec3 out_bitangent;

uniform sampler2D texWater;

void main()
{
	if(plsDiscard != uint(0))
		discard;
	/*{
		gl_FragColor = vec4(1.0);
		return;
	}*/
	vec3 normNormal = normalize(out_normal);
	if(shaderType == uint(2))
	{
		// water shader
		mat3 rotMatrix = bumpGetRotation(normNormal, out_bitangent);
		normNormal = bumpReadNormal(texture(texWater,vec2(0.0)).xyz, rotMatrix);
	}
	
	vec3 color = renderMapBlock(mapPos, normNormal, diffColor, specColor.rgb, specColor.w);
	
	if(shaderType == uint(0))
	{
		gl_FragColor = vec4(correctGamma(color),1.0);
	}
	else
	{
		//if(cubeSide == uint(16))
			//color.r = 1.0;
		gl_FragColor = vec4(clamp(correctGamma(color) * texture(texWater,vec2(0.5)).rgb,0.0,1.0), 0.5);
	}
}