#version 330 core

#include "light/BlockRenderer.glsl"
#include "uniforms/Material.glsl"

in vec4 normal;
in vec3 mapPos;

void main()
{
	vec3 color = renderMapBlock(mapPos, normalize(normal.xyz), MaterialDiffuse, MaterialSpecular, MaterialGlossy);
	//gl_FragColor = vec4(MaterialDiffuse, 1.0);
	gl_FragColor = vec4(color,1.0);
}