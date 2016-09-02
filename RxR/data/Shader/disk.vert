#version 330 core

#include "uniforms/Transforms.glsl"

in vec4 inPos;
out vec2 curPos;
out vec2 screen;

void main() 
{
	curPos = inPos.zw;
	vec4 pos = matModel * vec4(inPos.xy, 0.0, 1.0);
	gl_Position = matProjection * matCamera * pos;
	screen = pos.xy;
}