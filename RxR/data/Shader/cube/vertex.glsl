#include "../uniforms/Transforms.glsl"
#include "../uniforms/Framework.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord0;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in ivec3 in_iinfo;

uniform uint animation;

#define CHUNK_SIZE 32

layout(location = 0) out vec3 out_mapPos;
layout(location = 1) out vec2 out_texCoord0;
layout(location = 2) flat out vec3 out_normal;
layout(location = 3) flat out vec3 out_diffColor;
layout(location = 4) flat out vec4 out_specColor;
layout(location = 5) flat out uint out_shaderType;
layout(location = 6) flat out uint out_cubeSide;
layout(location = 7) flat out uint out_neighbors;
layout(location = 8) flat out vec3 out_tangent;
layout(location = 9) flat out float out_glowing;

void main()
{
	out_neighbors  = (uint(in_iinfo.z) & uint(0xFC000000)) >> 26;
	out_cubeSide = uint(0);
	
	out_normal = in_normal;
	
	if(out_normal.x != 0.0)
	{
		if(out_normal.x > 0.0) // right
			out_cubeSide = uint(2);
		else
			out_cubeSide = uint(1);
	}
	else if(out_normal.y != 0.0)
	{
		if(out_normal.y > 0.0) // top
			out_cubeSide = uint(4);
		else
			out_cubeSide = uint(8);
	}
	else // z != 0
	{
		if(out_normal.z > 0.0) // front
			out_cubeSide = uint(16);
		else
			out_cubeSide = uint(32);
	}
	
	if((out_neighbors & out_cubeSide) != uint(0))
	{
		// move out of window
		gl_Position = vec4(-2.0,-2.0,0.0,1.0);
		return;
	}
	out_tangent = in_tangent;
	
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
	
	out_diffColor = vec3(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0);
	
	r = (uint(in_iinfo.y) & uint(0xFF00)) >> 8;
	g = (uint(in_iinfo.y) & uint(0xFF0000)) >> 16;
	b = (uint(in_iinfo.y) & uint(0xFF000000)) >> 24;
	uint igloss = uint(in_iinfo.z);
	
	out_specColor = vec4(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0, float(igloss));
	
	out_shaderType = (uint(in_iinfo.z) & uint(0x070000)) >> 16;
	out_glowing = float((uint(in_iinfo.z) & uint(0x080000)) >> 19);
	
	vec3 inChunkPos = in_position * 0.5 + chOffset;
	
	if(animation == uint(0))
	{
		out_normal = (matModel * vec4(out_normal,0.0)).xyz;
		out_mapPos = (matModel * vec4(inChunkPos, 1.0)).xyz;
		gl_Position = matProjection * matCamera * matModel * vec4(inChunkPos, 1.0);
	}
	else // wind animation
	{
		// draw cube from back to front -> flip out_cubeSides
		float xfactor = sin(framework.random.z * 6.28318531) * 7.0;
		float zfactor = cos(framework.random.y * 6.28318531) * 7.0;
		float h = 1.0 - (inChunkPos.y + 0.5) / float(CHUNK_SIZE); // [0-1]
		
		inChunkPos.x = inChunkPos.x + h * xfactor;
		inChunkPos.z = inChunkPos.z + h * zfactor;
		
		out_normal = (matModel * vec4(out_normal,0.0)).xyz;
		out_mapPos = (matModel * vec4(inChunkPos, 1.0)).xyz;
		gl_Position = matProjection * matCamera * matModel * vec4(inChunkPos, 1.0);
	}
	out_normal = normalize(out_normal);
	// flow animation
	switch(out_cubeSide)
	{
	case uint(4):
	case uint(8): // top + bottom
		out_texCoord0 = out_mapPos.xz / vec2(16.0) + framework.random.xy;
		break;
	case uint(1): // left rigt
	case uint(2):
		out_texCoord0 = (out_mapPos.zy + vec2(8.0)) / vec2(16.0) + framework.random.xy;
	default: // front back
		out_texCoord0 = out_mapPos.xy / vec2(16.0) - vec2(0.0,framework.random.w);
		
	}
}