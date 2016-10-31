#version 330 core

#include "uniforms/Transforms.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in uint in_iinfo;

out vec4 normal;
out vec3 mapPos;
flat out vec3 cubeMidpoint;
flat out vec3 diffColor;
flat out vec4 specColor;

// matModel -> origin of chunk
uniform uint chunkHeight;

void main()
{
	//matIndex = in_iinfo.z + in_iinfo.w * uint(256);
	uint chIndex = (in_iinfo & uint(0xFFFF));
	uvec3 chPos;
	chPos.z = chIndex / (uint(16) * chunkHeight);
	chPos.y = (chIndex % (uint(16) * chunkHeight)) / uint(16);
	chPos.x = (chIndex % (uint(16) * chunkHeight)) % uint(16);

	vec3 chOffset = vec3(chPos);//vec3(float(in_iinfo.x), float(in_iinfo.y), float(in_iinfo.z));
	//chOffset.y = 0.0;
	//chOffset.z = 0.0;
	//chOffset.x = chOffset.x;
	
	diffColor = vec3(1.0);
	specColor = vec4(1.0);
	
	normal = matModel * vec4(in_normal,0.0);
	mapPos = (matModel * vec4(in_position * 0.5 + chOffset, 1.0)).xyz;
	cubeMidpoint = (matModel * vec4(chOffset, 1.0)).xyz;
	gl_Position = matProjection * matCamera * matModel * vec4(in_position * 0.5 + chOffset, 1.0);
}