#version 330 core

#include "uniforms/Transforms.glsl"

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec4 in_color;

out vec4 fcolor;

void main()
{
	fcolor = in_color;
	gl_Position = matProjection * matCamera * matModel * vec4(in_pos, 1.0);
}