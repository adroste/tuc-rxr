#version 330 core

uniform vec4 color;

in vec2 pos;

out vec4 colorResult;

void main()
{
	float x = 1.0 - (pos.x + 1.0) / 2.0;
	float y = (pos.y + 1.0) / 2.0;
	vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
	
	colorResult = mix(mix(color, white, x), black, y);
}