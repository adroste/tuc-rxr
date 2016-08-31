#version 330 core

in vec2 curPos;
uniform vec4 color;

void main()
{
	if (length(curPos) > 1.0)
		discard;
	gl_FragColor = color;
}