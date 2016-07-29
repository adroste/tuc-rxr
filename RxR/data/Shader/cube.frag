#version 330 core

#include "light/BlockRenderer.glsl"
#include "uniforms/Material.glsl"
#include "light/gamma.glsl"

in vec4 normal;
in vec3 mapPos;
flat in vec3 cubeMidpoint;

void main()
{
	vec3 normNormal = normalize(normal.xyz);
	vec3 neighbourBlock = cubeMidpoint + normNormal;
	
	if(isInMap(neighbourBlock) && getMapVolumeValue(neighbourBlock) >= 0.5)
		discard;
	
	vec3 color = renderMapBlock(mapPos, normNormal, MaterialDiffuse, MaterialSpecular, MaterialGlossy);
	
	gl_FragColor = vec4(correctGamma(color),1.0);
}