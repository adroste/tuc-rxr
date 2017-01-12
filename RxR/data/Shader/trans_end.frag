#version 330 core

out vec4 fragColor[2];

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	vec4 accum = texelFetch(tex0, ivec2(gl_FragCoord.xy), 0);
	float r = accum.a;
	vec2 info = texelFetch(tex1, ivec2(gl_FragCoord.xy), 0).rg;
	vec4 finalColor = vec4(accum.rgb / clamp(info.r, 1e-4, 5e4), r);
	fragColor[0] = finalColor;
	fragColor[1] = info.g * finalColor;
}