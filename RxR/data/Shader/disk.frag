#version 330 core

#include "uniforms/Framework.glsl"

uniform vec4 color;

in vec2 curPos;
in vec2 screen;

void main(void) 
{
	if(!framework_isInRect(screen)) discard;
	if (length(curPos) > 1.0)
		discard;
	gl_FragColor = color;
}