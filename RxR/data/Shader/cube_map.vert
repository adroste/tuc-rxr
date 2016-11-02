#version 330 core

#include "uniforms/Transforms.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in ivec3 in_iinfo;
#define CHUNK_SIZE 32

out vec4 normal;
out vec3 mapPos;
//flat out vec3 cubeMidpoint;
flat out vec3 diffColor;
flat out vec4 specColor;
flat out uint shaderType;

void main()
{
	uint chIndex = (uint(in_iinfo.x) & uint(0xFFFF));
	uvec3 chPos;
	chPos.z = chIndex / (uint(CHUNK_SIZE) * uint(CHUNK_SIZE));
	chPos.y = (chIndex % (uint(CHUNK_SIZE) * uint(CHUNK_SIZE))) / uint(CHUNK_SIZE);
	chPos.x = (chIndex % (uint(CHUNK_SIZE) * uint(CHUNK_SIZE))) % uint(CHUNK_SIZE);
	
	vec3 chOffset = vec3(chPos);//vec3(float(in_iinfo.x), float(in_iinfo.y), float(in_iinfo.z));
	
	// extract color
	uint r = (uint(in_iinfo.x) & uint(0xFF000000)) >> 24;
	uint g = (uint(in_iinfo.x) & uint(0xFF0000)) >> 16;
	uint b = (uint(in_iinfo.y) & uint(0xFF));
	
	diffColor = vec3(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0);
	
	r = (uint(in_iinfo.y) & uint(0xFF00)) >> 8;
	g = (uint(in_iinfo.y) & uint(0xFF0000)) >> 16;
	b = (uint(in_iinfo.y) & uint(0xFF000000)) >> 24;
	uint igloss = uint(in_iinfo.z);
	
	specColor = vec4(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0, float(igloss));
	
	shaderType = (uint(in_iinfo.z) & uint(0xFF0000)) >> 16;
	
	normal = matModel * vec4(in_normal,0.0);
	mapPos = (matModel * vec4(in_position * 0.5 + chOffset, 1.0)).xyz;
	//cubeMidpoint = (matModel * vec4(chOffset, 1.0)).xyz;
	gl_Position = matProjection * matCamera * matModel * vec4(in_position * 0.5 + chOffset, 1.0);
}