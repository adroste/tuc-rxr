#version 330 core

uniform sampler2D tex0;
in vec2 texCoord;

void main()
{
	gl_FragColor = texture(tex0,texCoord);
}