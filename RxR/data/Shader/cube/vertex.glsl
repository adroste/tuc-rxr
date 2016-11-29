#include "../uniforms/Transforms.glsl"
#include "../uniforms/Framework.glsl"

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord0;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in ivec3 in_iinfo;

uniform uint animation;

#define CHUNK_SIZE 32

out vec3 out_mapPos;
out vec2 texCoord0;
flat out vec3 out_normal;
flat out vec3 diffColor;
flat out vec4 specColor;
flat out uint shaderType;
flat out uint cubeSide;
flat out uint cubeNeighbors;
flat out vec3 out_bitangent;
flat out float isGlowing;

void main()
{
	uint neighbors  = (uint(in_iinfo.z) & uint(0xFC000000)) >> 26;
	cubeNeighbors = neighbors;
	uint side = uint(0);
	
	vec3 normal = in_normal;
	vec3 pos = in_position;
	
	if(normal.x != 0.0)
	{
		if(normal.x > 0.0) // right
			side = uint(2);
		else
			side = uint(1);
	}
	else if(normal.y != 0.0)
	{
		if(normal.y > 0.0) // top
			side = uint(4);
		else
			side = uint(8);
	}
	else // z != 0
	{
		if(normal.z > 0.0) // front
			side = uint(16);
		else
			side = uint(32);
	}
	cubeSide = side;
	uint plsDiscard = neighbors & cubeSide;
	if(plsDiscard != uint(0))
	{
		// move outside window
		gl_Position = vec4(-2.0,-2.0,0.0,1.0);
		return;
	}
	out_bitangent = in_tangent;
	
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
	
	shaderType = (uint(in_iinfo.z) & uint(0x070000)) >> 16;
	uint glow = (uint(in_iinfo.z) & uint(0x080000)) >> 19;
	isGlowing = float(glow);
	
	vec3 inChunkPos = pos * 0.5 + chOffset;
	vec3 mapPos;
	
	if(animation == uint(0))
	{
		out_normal = (matModel * vec4(normal,0.0)).xyz;
		mapPos = (matModel * vec4(inChunkPos, 1.0)).xyz;
		gl_Position = matProjection * matCamera * matModel * vec4(inChunkPos, 1.0);
	}
	else // wind animation
	{
		// draw cube from back to front -> flip sides
		float xfactor = sin(framework.random.z * 6.28318531) * 7.0;
		float zfactor = cos(framework.random.y * 6.28318531) * 7.0;
		float h = 1.0 - (inChunkPos.y + 0.5) / float(CHUNK_SIZE); // [0-1]
		
		inChunkPos.x = inChunkPos.x + h * xfactor;
		inChunkPos.z = inChunkPos.z + h * zfactor;
		
		out_normal = (matModel * vec4(normal,0.0)).xyz;
		mapPos = (matModel * vec4(inChunkPos, 1.0)).xyz;
		gl_Position = matProjection * matCamera * matModel * vec4(inChunkPos, 1.0);
	}
	out_mapPos = mapPos;
	
	switch(side)
	{
	case uint(4):
	case uint(8): // top + bottom
		texCoord0 = mapPos.xz / vec2(16.0) + framework.random.xy;
		break;
	case uint(1): // left rigt
	case uint(2):
		texCoord0 = (mapPos.zy + vec2(8.0)) / vec2(16.0) + framework.random.xy;
	default: // front back
		texCoord0 = mapPos.xy / vec2(16.0) - vec2(0.0,framework.random.w);
		
	}
}