#version 330 core

#include "light/BlockRenderer.glsl"
#include "uniforms/Material.glsl"
#include "light/gamma.glsl"

in vec4 normal;
in vec3 mapPos;
//flat in vec3 cubeMidpoint;
flat in vec3 diffColor;
flat in vec4 specColor;
flat in uint shaderType;

void main()
{
	vec3 normNormal = normalize(normal.xyz);
	//vec3 neighbourBlock = cubeMidpoint + normNormal;
	
	//if(isInMap(neighbourBlock) && getMapVolumeValue(neighbourBlock) >= 0.5)
		//discard;
	
	vec3 color = renderMapBlock(mapPos, normNormal, diffColor, specColor.rgb, specColor.w);
	
	if(shaderType == uint(0))
	{
		gl_FragColor = vec4(correctGamma(color),0.0);
		gl_FragDepth = gl_FragCoord.z;
	}
	else
	{
		gl_FragDepth = 0.99;
		
		gl_FragColor = vec4(correctGamma(color) * 0.8, 0.2);
	}
}