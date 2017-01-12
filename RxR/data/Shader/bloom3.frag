#version 330 core

// normal rgb texture
uniform sampler2D tex0;
// texture that will be added
uniform sampler2D tex1;

out vec4 fragColor;

void main()
{
	ivec2 texCoord = ivec2(gl_FragCoord.xy);
	fragColor = texelFetch(tex0,texCoord,0) + texelFetch(tex1,texCoord,0);
}