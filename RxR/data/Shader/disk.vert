#version 330 core

#include "uniforms/Transforms.glsl"

in vec4 inPos;
out vec2 curPos;

void main() 
{
	curPos = inPos.zw;
	gl_Position = matProjection * matCamera * matModel * vec4(inPos.xy, 0.0, 1.0);
}