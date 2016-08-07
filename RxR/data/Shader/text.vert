#version 330 core

#include "uniforms/Transforms.glsl"

in vec4 textInfo;

out vec2 texpos;

void main(void) 
{
	gl_Position = matProjection * matCamera * matModel * vec4(textInfo.xy, 0, 1);
	texpos = textInfo.zw;
}