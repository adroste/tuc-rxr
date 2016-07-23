#version 330 core

#include "uniforms/Transforms.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

out vec4 normal;

void main()
{
	normal = matCamera * matModel * vec4(in_normal,0.0);
	gl_Position = matProjection * matCamera * matModel * vec4(in_position,1.0);
}