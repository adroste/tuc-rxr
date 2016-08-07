#version 330 core

#include "uniforms/Transforms.glsl"

in vec4 btnInfo;

out vec2 texCoord;
out vec3 pos;

void main()
{
	texCoord = btnInfo.zw;
	vec4 p = vec4(btnInfo.xy, 0.0, 1.0);
	pos = p.xyz;
	gl_Position = matProjection * matCamera * matModel * p;
}