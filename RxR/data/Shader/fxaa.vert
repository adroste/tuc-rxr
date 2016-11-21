#version 420 core

layout(location = 0) in vec2 in_pos;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(in_pos,0.0,1.0);
	texCoord = (in_pos + vec2(1.0)) / 2.0;
}