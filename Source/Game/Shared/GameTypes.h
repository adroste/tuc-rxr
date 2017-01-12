#pragma once
#include "../../ecs/entitycs.h"
#include "../../glm/glm.hpp"
#include "CubeDesc.h"

using HealthT = int_fast32_t;
using ColorT = uint32_t;

struct Transform
{
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec2 rortation;
};

struct Movement
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
};

struct Health
{
	HealthT hp;
	HealthT armor;
};

struct Damage
{
	HealthT truedmg;
	HealthT magicmg;
	HealthT physicaldmg;
};

struct CubeShape
{
	ColorT diffuse;
	ColorT specular;
	float gloss;
	uint_least8_t flags;
	CubeShader shader;
	BlockType type;
};

struct MapChunkInfo
{
	uint_least8_t neighbors;
};

using GameManager =	ecs::Manager<Transform, Movement, Health, Damage, CubeShape, MapChunkInfo>;
using GameEntity =	ecs::Entity< Transform, Movement, Health, Damage, CubeShape, MapChunkInfo>;
using GameSystem =	ecs::System< Transform, Movement, Health, Damage, CubeShape, MapChunkInfo>;
using GameScript =	ecs::Script< Transform, Movement, Health, Damage, CubeShape, MapChunkInfo>;