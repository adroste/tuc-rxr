#include "../light/BlockRenderer.glsl"
#include "../uniforms/Material.glsl"
#include "../light/gamma.glsl"
#include "../light/bump.glsl"

in vec3 out_mapPos;
in vec2 texCoord0;
flat in vec3 out_normal;
flat in vec3 diffColor;
flat in vec4 specColor;
flat in uint shaderType;
flat in uint cubeSide;
flat in uint cubeNeighbors;
flat in vec3 out_bitangent;
flat in float isGlowing;

layout(binding = 1) uniform sampler2D texWater;
layout(binding = 2) uniform sampler2D texWaterfall;

void getPixelColor(out vec4 dstColor, out bool dstGlowing)
{
	vec3 normNormal = normalize(out_normal);
	if(shaderType == uint(2))
	{
		// flip water normal if facing away from eyeDir
		vec3 eyeDir = normalize(LightsEye - out_mapPos);
		if (dot(normNormal, eyeDir) < 0.0)
			normNormal *= -1.0;
		// water shader
		vec3 rgbNormal;
		if(cubeSide == uint(4) || cubeSide == uint(8))
			rgbNormal = texture(texWater,texCoord0).xyz;
		else
			rgbNormal = texture(texWaterfall,texCoord0).xyz;
			
		mat3 rotMatrix = bumpGetRotation(normNormal, out_bitangent);
		normNormal = bumpReadNormal(rgbNormal, rotMatrix);
	}
	
	vec3 color = renderMapBlock(out_mapPos, normNormal, diffColor, specColor.rgb, specColor.w);

	if(shaderType == uint(0))
	{
		dstColor = vec4(correctGamma(color),1.0);
	}
	else if(shaderType == uint(1))
	{
		dstColor = vec4(clamp(correctGamma(color), 0.0,1.0), 0.5);
	}
	else
	{
		vec3 eyeDir = normalize(LightsEye - out_mapPos);
		
		float fangle = 1.0 + dot(eyeDir,normNormal);
		fangle = pow(fangle,3);
		fangle = clamp(1.0 / fangle,0.5,0.9);
		
		dstColor = vec4(clamp(correctGamma(color), 0.0,1.0), fangle);
	}
	
	dstGlowing = (isGlowing > 0.0);
}