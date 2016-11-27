#version 420 core

layout(binding = 0) uniform sampler2D accumTex;
layout(binding = 1) uniform sampler2D revealTex;

out vec4 fragColor[2];

void main()
{
	vec4 accum = texelFetch(accumTex, ivec2(gl_FragCoord.xy), 0);
	float r = accum.a;
	vec2 info = texelFetch(revealTex, ivec2(gl_FragCoord.xy), 0).rg;
	accum.a = info.r;
	vec4 finalColor = vec4(accum.rgb / clamp(accum.a, 1e-4, 5e4), r);
	fragColor[0] = finalColor;
	fragColor[1] = info.g * finalColor;
}