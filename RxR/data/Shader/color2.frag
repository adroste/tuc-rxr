#version 330 core

#include "light/gamma.glsl"
#include "uniforms/Framework.glsl"

in vec4 fcolor;
in vec2 screen;

void main(void) 
{
	if(!framework_isInRect(screen)) discard;
	gl_FragColor = fcolor;
}