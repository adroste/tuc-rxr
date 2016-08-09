#version 330 core

#include "light/gamma.glsl"

in vec4 fcolor;

void main()
{
	//gl_FragColor = vec4(correctGamma(fcolor.xyz), 1.0);
	gl_FragColor = fcolor;
}