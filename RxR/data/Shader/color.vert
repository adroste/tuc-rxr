#version 330 core

#include "uniforms/Transforms.glsl"

in vec3 in_pos;
out vec2 screen;

void main()
{
	vec4 pos = matModel * vec4(in_pos, 1.0);
	gl_Position = matProjection * matCamera * pos;
	screen = pos.xy;
}