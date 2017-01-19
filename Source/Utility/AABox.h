#pragma once
#include "../glm/glm.hpp"

// Axis aligned box
struct AABox
{
	union
	{
		glm::vec3 p1;
		struct
		{
			float xmin;
			float ymin;
			float zmin;
		};
	};
	union
	{
		glm::vec3 p2;
		struct
		{
			float xmax;
			float ymax;
			float zmax;
		};
	};
	AABox(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
		:
		xmin(xmin), ymin(ymin), zmin(zmin), xmax(xmax), ymax(ymax), zmax(zmax)
	{}
	AABox(const glm::vec3& p1, const glm::vec3& p2)
		:
		p1(p1), p2(p2)
	{}
	AABox()
	{}
};