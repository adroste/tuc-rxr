#version 420 core
#include "uniforms/Framebuffer.glsl"

// image with blooming parts only
layout(binding = 0) uniform sampler2D tex0;
// depth information in g
layout(binding = 1) uniform sampler2D tex1;

uniform vec2 dir; // [1.0,0.0] or [0.0,1.0]

in vec2 texCoord;
out vec4 fragColor;

const int gaussRadius = 11;
const float gaussFilter[gaussRadius] = float[gaussRadius](
	0.0402,0.0623,0.0877,0.1120,0.1297,0.1362,0.1297,0.1120,0.0877,0.0623,0.0402
);

void main()
{
	// pick texel on the left/top
	vec2 texel = texCoord - float(int(gaussRadius/2)) * dir * frame.pstep;
	
	vec3 sum = vec3(0.0);
	for(int i = 0; i < gaussRadius; i++)
	{
		sum += gaussFilter[i] * texture(tex0, texel).rgb;
		texel += dir * frame.pstep;
	}
	
	sum = min(sum, vec3(1.0));
	
	fragColor = vec4(sum,1.0);
}