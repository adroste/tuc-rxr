#version 330 core

#include "uniforms/Framebuffer.glsl"

uniform sampler2D tex0;
in vec2 texCoord;

#define FXAA_SPAN_MAX 2.5
#define FXAA_REDUCE_MUL 0.0
#define FXAA_REDUCE_MIN (1.0 / 128.0)
#define FXAA_SUBPIX_SHIFT 1.0

float fxaaLuma(in vec3 rgb)
{
// other luma: 	vec3( 0.299 , 0.587 , 0.114);
	return rgb.g * (0.587/0.299) + rgb.r; 
}

vec3 fxaaPixelShader()
{
	/*
		NW		NE
			M
		SW		SE
	*/
	vec2 s = frame.pstep;
	
	vec3 rgbM = texture(tex0 , texCoord).rgb;
	vec3 rgbNW = texture(tex0, texCoord - (s * FXAA_SUBPIX_SHIFT)).rgb;
	vec3 rgbNE = texture(tex0, texCoord + (vec2(s.x, -s.y) * FXAA_SUBPIX_SHIFT)).rgb;
	vec3 rgbSW = texture(tex0, texCoord + (vec2(-s.x, s.y) * FXAA_SUBPIX_SHIFT)).rgb;
	vec3 rgbSE = texture(tex0, texCoord + (s * FXAA_SUBPIX_SHIFT)).rgb;
	
	// edge detection
	float lumaM =  fxaaLuma(rgbM);
	float lumaNW = fxaaLuma(rgbNW);
	float lumaNE = fxaaLuma(rgbNE);
	float lumaSW = fxaaLuma(rgbSW);
	float lumaSE = fxaaLuma(rgbSE);
	
	float lumaMin = min(lumaM, min(lumaNW, min( lumaNE, min( lumaSW, lumaSW))));
	float lumaMax = max(lumaM, max(lumaNW, max( lumaNE, max( lumaSW, lumaSW))));
	
	vec2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
	
	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
	
	// adjust dir magnitude
	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = clamp(dir * rcpDirMin, vec2(-FXAA_SPAN_MAX),vec2(FXAA_SPAN_MAX)) * s;
	
	vec3 rgbA = (1.0 / 2.0) * (texture(tex0, texCoord + dir * (1.0 / 3.0 - 0.5)).rgb +
		texture(tex0, texCoord + dir * (2.0 / 3.0 - 0.5)).rgb);
	vec3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (texture(tex0, texCoord + dir * (0.0 / 3.0 - 0.5)).rgb + 
		texture(tex0, texCoord + dir * (3.0 / 3.0 - 0.5)).rgb);
	
	float lumaB = fxaaLuma(rgbB);
	if((lumaB < lumaMin) || (lumaB > lumaMax))
		return rgbA; // rgbB was outside of luma range
	else
		return rgbB;
}

void main()
{
	gl_FragColor = vec4(fxaaPixelShader(), 1.0);
	//gl_FragColor = texture(tex1, texCoord); //+ vec4(fxaaPixelShader(), 1.0);
}