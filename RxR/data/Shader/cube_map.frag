#version 410 core

#include "cube/fragHeader.glsl"

out vec4 fragColor[2];

void main()
{
	vec4 finalColor = vec4(0.0);
	vec4 glowColor = vec4(0.0);
	
	bool isGlowing = false;
	getPixelColor(finalColor, isGlowing);
	
	if(isGlowing)
		glowColor = vec4(in_diffColor,1.0);
	
	fragColor[0] = finalColor;
	fragColor[1] = glowColor;
}