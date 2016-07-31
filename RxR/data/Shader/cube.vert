#version 330 core

#include "uniforms/Transforms.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

out vec4 normal;
out vec3 mapPos;
flat out vec3 cubeMidpoint;

void main()
{
	normal = matModel * vec4(in_normal,0.0);
	mapPos = (matModel * vec4(in_position, 1.0)).xyz;
	cubeMidpoint = (matModel * vec4(vec3(0.0), 1.0)).xyz;
	gl_Position = matProjection * matCamera * matModel * vec4(in_position, 1.0);
}