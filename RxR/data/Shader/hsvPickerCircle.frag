#version 330 core

#include "light/gamma.glsl"

#define PI 3.1415926535897932384626433832795
#define PI2 6.283185307179586476925286766559

in vec2 pos;

void main()
{	
	if(length(pos) > 1.0 || length(pos) < 0.8)
		discard;
	
	float angle = acos(dot(normalize(pos), vec2(1.0, 0.0)));
	if(dot(normalize(pos), vec2(0.0, 1.0)) > 0)
		angle = PI2 - angle;
	
	float hue = angle / PI2;
	vec3 color;
	color.r = abs(6.0 * hue - 3.0) - 1.0;
	color.g = 2.0 - abs(6.0 * hue - 2.0);
	color.b = 2.0 - abs(6.0 * hue - 4.0);

	color = clamp(color, 0.0, 1.0);
	
	gl_FragColor = vec4(color, 1.0);
}