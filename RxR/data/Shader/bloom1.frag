#version 420 core

// rgb image
layout(binding = 0) uniform sampler2D tex0;
// glow and depth information
layout(binding = 1) uniform sampler2D tex1;
in vec2 texCoord;
out vec4 fragColor;

void main()
{
	vec4 color = texture(tex0, texCoord);
	vec4 info = texture(tex1, texCoord);
	
	fragColor = color * float(info.r);
}