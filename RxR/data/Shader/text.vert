#version 330 core

#include "uniforms/Transforms.glsl"

in vec4 textInfo;

out vec2 texpos;
out vec2 screen;

void main(void) 
{
	vec4 pos = matModel * vec4(textInfo.xy, 0, 1);
	gl_Position = matProjection * matCamera * pos;
	screen = pos.xy;
	texpos = textInfo.zw;
}