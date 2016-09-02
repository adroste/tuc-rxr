#version 330 core

#include "uniforms/Framework.glsl"

uniform vec4 color;
in vec2 screen;

void main(void) 
{
	if(!framework_isInRect(screen)) discard;
	gl_FragColor = color;
}