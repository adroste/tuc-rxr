#version 330 core

#include "light/BlockRenderer.glsl"
#include "uniforms/Material.glsl"
#include "light/gamma.glsl"
#include "light/bump.glsl"

in vec3 out_mapPos;
in vec2 texCoord0;
flat in vec3 out_normal;
flat in vec3 diffColor;
flat in vec4 specColor;
flat in uint shaderType;
flat in uint plsDiscard;
flat in uint cubeSide;
flat in uint cubeNeighbors;
flat in vec3 out_bitangent;
flat in float isGlowing;

uniform sampler2D texWater;
uniform sampler2D texWaterfall;

out vec4 fragColor[2];

void main()
{
	if(plsDiscard != uint(0))
		discard;
	/*{
		gl_FragColor = vec4(1.0);
		return;
	}*/
	vec3 normNormal = normalize(out_normal);
	if(shaderType == uint(2))
	{
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
		fragColor[0] = vec4(correctGamma(color),1.0);
	}
	else if(shaderType == uint(1))
	{
		fragColor[0] = vec4(clamp(correctGamma(color), 0.0,1.0), 0.5);
	}
	else
	{
		vec3 eyeDir = normalize(LightsEye - out_mapPos);
		
		float fangle = 1.0 + dot(eyeDir,normNormal);
		fangle = pow(fangle,5);
		fangle = clamp(1.0 / fangle,0.5,0.9);
		
		fragColor[0] = vec4(clamp(correctGamma(color), 0.0,1.0), /*1.0 - */fangle);
	}
	fragColor[1] = vec4(isGlowing, out_mapPos.z, 0.0, isGlowing);
}