#version 420 core

// normal rgb texture
layout(binding = 0) uniform sampler2D tex0;
// texture that will be added
layout(binding = 1) uniform sampler2D tex1;

in vec2 texCoord;
out vec4 fragColor;

void main()
{
	fragColor = min(texture(tex0,texCoord) + texture(tex1,texCoord),vec4(1.0));
}