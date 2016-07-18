#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

varying vec4 normal;

void main()
{
	normal = gl_ModelViewMatrix * vec4(in_normal,0.0);
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(in_position,1.0);
}