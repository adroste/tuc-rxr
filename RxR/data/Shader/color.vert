#version 330 core

#include "uniforms/Transforms.glsl"

in vec3 in_pos;

void main()
{
	gl_Position = matProjection * matCamera * matModel * vec4(in_pos, 1.0);
}