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
	// TODO do proper rotation
	AABox transform(const glm::mat4& m) const
	{
		AABox r;
		auto n1 = m * glm::vec4(p1,1.0f);
		auto n2 = m * glm::vec4(p2,1.0f);
		r.p1 = { n1.x,n1.y,n1.z };
		r.p2 = { n2.x,n2.y,n2.z };
		return r;
	}
};