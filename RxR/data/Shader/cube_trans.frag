#version 420 core
// transparent cubes
#include "cube/fragHeader.glsl"

out vec4 fragColor[2];

float getTransWeight(float z, float a)
{
	return a * pow(z,4.0);
}

void main()
{
	vec4 finalColor = vec4(0.0);
	vec4 glowColor = vec4(0.0);
	
	bool isGlowing = false;
	getPixelColor(finalColor, isGlowing);
	
	float glowFac = 0.0;
	if(isGlowing)
		glowFac = 1.0;
		
	float weight = getTransWeight(gl_FragCoord.z,finalColor.a);
	fragColor[0] = vec4( finalColor.rgb * weight, finalColor.a);
	fragColor[1] = vec4(finalColor.a * weight, glowFac, 0.0, 1.0);
}