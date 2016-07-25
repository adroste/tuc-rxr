// union with light sources
#define MAX_LIGHTS 20

struct LightSource
{
	uint type; // types:
	// 0: directional (sun rays)
	// 1: PointLight
	vec3 color;
	vec3 origin; // light direction if directional | light origin if PointLight
	float attenuation;
};

uniform Lights
{
	uint LightsNLights;
	vec3 LightsAmbient; // ambient scene color
	vec3 LightsEye;
	LightSource LightsLight[MAX_LIGHTS];
};