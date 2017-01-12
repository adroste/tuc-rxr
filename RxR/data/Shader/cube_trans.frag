#version 330 core
// transparent cubes
#include "cube/fragHeader.glsl"

out vec4 fragColor[2];

float getTransWeight(float z, float a)
{
	return a * pow(z,4.0);
}

float clampZ(float z)
{
	// map should be from -0.5 to 31.5
	// linear clamp from ~ -10 to 100 to be sure
	
	return 1.0 - pow((z + 16.0) / 128.0, 4.0);
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
		
	float weight = getTransWeight(clampZ(in_mapPos.z),finalColor.a);
	fragColor[0] = vec4( finalColor.rgb * weight, finalColor.a);
	fragColor[1] = vec4(finalColor.a * weight, glowFac, 0.0, 1.0);
}