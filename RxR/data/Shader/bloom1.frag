#version 330 core

// rgb image
uniform sampler2D tex0;
// glow and depth information
uniform sampler2D tex1;

out vec4 fragColor;

void main()
{
	ivec2 texCoord = ivec2(gl_FragCoord.xy);
	vec4 color = texelFetch(tex0, texCoord, 0);
	vec4 info = texelFetch(tex1, texCoord, 0);
	
	fragColor = color * float(info.r);
}