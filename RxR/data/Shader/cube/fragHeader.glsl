#include "../light/BlockRenderer.glsl"
#include "../uniforms/Material.glsl"
#include "../light/gamma.glsl"
#include "../light/bump.glsl"

layout(location = 0) in vec3 in_mapPos;
layout(location = 1) in vec2 in_texCoord0;
layout(location = 2) flat in vec3 in_normal;
layout(location = 3) flat in vec3 in_diffColor;
layout(location = 4) flat in vec4 in_specColor;
layout(location = 5) flat in uint in_shaderType;
layout(location = 6) flat in uint in_cubeSide;
layout(location = 7) flat in uint in_neighbors;
layout(location = 8) flat in vec3 in_tangent;
layout(location = 9) flat in float in_glowing;

layout(binding = 1) uniform sampler2D texWater;
layout(binding = 2) uniform sampler2D texWaterfall;

void getPixelColor(out vec4 dstColor, out bool dstGlowing)
{
	vec3 normNormal = in_normal;
	if(in_shaderType == uint(2))
	{
		// flip water normal if facing away from eyeDir
		vec3 eyeDir = normalize(LightsEye - in_mapPos);
		if (dot(normNormal, eyeDir) < 0.0)
			normNormal *= -1.0;
		// water shader
		vec3 rgbNormal;
		if(in_cubeSide == uint(4) || in_cubeSide == uint(8))
			rgbNormal = texture(texWater,in_texCoord0).xyz;
		else
			rgbNormal = texture(texWaterfall,in_texCoord0).xyz;
			
		mat3 rotMatrix = bumpGetRotation(normNormal, in_tangent);
		normNormal = bumpReadNormal(rgbNormal, rotMatrix);
	}
	
	vec3 color = renderMapBlock(in_mapPos, normNormal, in_diffColor, in_specColor.rgb, in_specColor.w);

	if(in_shaderType == uint(0))
	{
		dstColor = vec4(correctGamma(color),1.0);
	}
	else if(in_shaderType == uint(1))
	{
		dstColor = vec4(clamp(correctGamma(color), 0.0,1.0), 0.5);
	}
	else
	{
		vec3 eyeDir = normalize(LightsEye - in_mapPos);
		
		float fangle = 1.0 + dot(eyeDir,normNormal);
		fangle = pow(fangle,3);
		fangle = clamp(1.0 / fangle,0.5,0.9);
		
		dstColor = vec4(clamp(correctGamma(color), 0.0,1.0), fangle);
	}
	
	dstGlowing = (in_glowing > 0.0);
}