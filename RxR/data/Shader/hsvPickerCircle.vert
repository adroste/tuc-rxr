#version 330 core

#include "uniforms/Transforms.glsl"

in vec4 info;

out vec2 pos;

void main()
{
	gl_Position = matProjection * matCamera * matModel * vec4(info.xy, 0, 1);
	pos = info.zw;
}