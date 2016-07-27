#version 330 core

uniform sampler3D blockTex;

#include "light/BlockRenderer.glsl"
#include "uniforms/Material.glsl"

in vec4 normal;
in vec3 mapPos;

void main()
{
	vec3 color = renderMapBlock(mapPos, normalize(normal.xyz), MaterialDiffuse, MaterialSpecular, MaterialGlossy);
	//gl_FragColor = vec4(MaterialDiffuse, 1.0);
	vec3 texPos = mapPos;
	texPos.x = texPos.x /= 50.0;
	texPos.y = texPos.y /= 35.0;
	texPos.z = texPos.z /= 4.0;
	color = texture3D(blockTex, texPos).rgb;
	if(color.r < 0.45)
		//color = vec3(0.0,0.0,1.0);
		discard;
	gl_FragColor = vec4(color,1.0);
}