#version 330 core

#include "uniforms/Framework.glsl"

uniform sampler2D tex;
uniform vec4 color;

in vec2 texpos;
in vec2 screen;

void main(void) 
{
	if(!framework_isInRect(screen)) discard;

	vec4 col = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;
	gl_FragColor = col;
}