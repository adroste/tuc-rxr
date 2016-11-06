#version 330 core

#include "uniforms/Transforms.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in ivec3 in_iinfo;

uniform uint animation;

#define CHUNK_SIZE 32

out vec4 normal;
out vec3 mapPos;
flat out vec3 diffColor;
flat out vec4 specColor;
flat out uint shaderType;
flat out uint plsDiscard;
flat out uint cubeSide;
flat out uint cubeNeighbors;

void main()
{
	uint chIndex = (uint(in_iinfo.x) & uint(0xFFFF));
	uvec3 chPos;
	chPos.z = chIndex / (uint(CHUNK_SIZE) * uint(CHUNK_SIZE));
	chPos.y = (chIndex % (uint(CHUNK_SIZE) * uint(CHUNK_SIZE))) / uint(CHUNK_SIZE);
	chPos.x = (chIndex % (uint(CHUNK_SIZE) * uint(CHUNK_SIZE))) % uint(CHUNK_SIZE);
	
	vec3 chOffset = vec3(chPos);
	
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
	uint neighbors  = (uint(in_iinfo.z) & uint(0xFC000000)) >> 26;
	cubeNeighbors = neighbors;
	uint side = uint(0);
	if(in_normal.x != 0.0)
	{
		if(in_normal.x > 0.0) // right
			side = uint(2);
		else
			side = uint(1);
	}
	else if(in_normal.y != 0.0)
	{
		if(in_normal.y > 0.0) // top
			side = uint(4);
		else
			side = uint(8);
	}
	else // z != 0
	{
		if(in_normal.z > 0.0) // front
			side = uint(16);
		else
			side = uint(32);
	}
	cubeSide = side;
	plsDiscard = neighbors & cubeSide;
	vec3 inChunkPos = in_position * 0.5 + chOffset;
	
	if(animation == uint(0))
	{
		normal = matModel * vec4(in_normal,0.0);
		mapPos = (matModel * vec4(inChunkPos, 1.0)).xyz;
		gl_Position = matProjection * matCamera * matModel * vec4(inChunkPos, 1.0);
	}
	else // wind animation
	{
		float sfac = 31.0 - chOffset.y;
		mat2 s = mat2(1.0, 2.0,0.0,sfac);
		inChunkPos.xy = s * inChunkPos.xy;
		
		normal = matModel * vec4(in_normal,0.0);
		mapPos = (matModel * vec4(inChunkPos, 1.0)).xyz;
		gl_Position = matProjection * matCamera * matModel * vec4(inChunkPos, 1.0);
	}
}