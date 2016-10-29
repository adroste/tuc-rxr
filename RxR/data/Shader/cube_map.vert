#version 330 core

#include "uniforms/Transforms.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in ivec4 in_iinfo;

out vec4 normal;
out vec3 mapPos;
flat out vec3 cubeMidpoint;
flat out uint matIndex;

// matModel -> origin of chunk
uniform uint chunkHeight;

void main()
{
	matIndex = in_iinfo.z + in_iinfo.w * uint(256);
	uint chIndex = in_iinfo.x + in_iinfo.y * uint(256);
	ivec3 chPos;
	chPos.z = chIndex / (uint(16) * chunkHeight);
	chPos.y = (chIndex % (uint(16) * chunkHeight)) / 16;
	chPos.z = (chIndex % (uint(16) * chunkHeight)) % 16;
	
	vec3 chOffset = vec3(chPos);

	normal = matModel * vec4(in_normal,0.0);
	mapPos = (matModel * vec4(in_position + chOffset, 1.0)).xyz;
	cubeMidpoint = (matModel * vec4(chOffset, 1.0)).xyz;
	gl_Position = matProjection * matCamera * matModel * vec4(in_position + chOffset, 1.0);
}