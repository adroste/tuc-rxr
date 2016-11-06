#version 330 core

#include "light/BlockRenderer.glsl"
#include "uniforms/Material.glsl"
#include "light/gamma.glsl"

in vec3 mapPos;
flat in vec4 normal;
flat in vec3 diffColor;
flat in vec4 specColor;
flat in uint shaderType;
flat in uint plsDiscard;
flat in uint cubeSide;
flat in uint cubeNeighbors;

void main()
{
	if(plsDiscard != uint(0))
		discard;

	vec3 normNormal = normalize(normal.xyz);
	
	vec3 color = renderMapBlock(mapPos, normNormal, diffColor, specColor.rgb, specColor.w);
	
	if(shaderType == uint(0))
	{
		gl_FragColor = vec4(correctGamma(color),0.0);
		gl_FragDepth = gl_FragCoord.z;
	}
	else
	{
		float depth = 0.99;
		if((cubeNeighbors & uint(1)) != uint(0))
		{
			// has left neighbor
			
		}
		
		
		gl_FragDepth = depth;
		gl_FragColor = vec4(correctGamma(color) * 0.8, 0.2);
	}
}