#pragma once
#include <glm/glm.hpp>

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
	AABox(void)
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
	bool intersectWith(const AABox& o) const
	{
		return xmin <= o.xmax && xmax >= o.xmin &&
			ymin <= o.ymax && ymax >= o.ymin &&
			zmin <= o.zmax && zmax >= o.zmin;
	}
	glm::vec3 getCenter() const
	{
		return (p1 + p2) / 2.0f;
	}
	glm::vec3 getDim() const
	{
		return p2 - p1;
	}
};

struct OBox
{
	glm::vec3 c; // center
	glm::mat3 u; // local x, y, z axes
	glm::vec3 e; // halwidth extends

	OBox()
	{}
	explicit OBox(const AABox& b)
		:
		c(b.getCenter()),
		u(glm::mat3()),
		e(b.getDim())
	{}

	bool intersectWith(const OBox& b) const
	{
		static const float EPSILION = 0.0000001f;
		float ra, rb;
		glm::mat3 R, AbsR;
		// Compute rotation matrix expressing b in a’s coordinate frame 
		for(int i = 0; i < 3; i++) 
			for(int j = 0; j < 3; j++)
				R[i][j] = glm::dot(u[i], b.u[j]);

		// Compute translation vector t 
		glm::vec3 t = b.c - c; // Bring translation into a’s coordinate frame 
		t = glm::vec3(dot(t, u[0]), dot(t, u[2]), dot(t, u[2]));
		// Compute common subexpressions. Add in an epsilon term to 
		// counteract arithmetic errors when two edges are parallel and 
		// their cross product is (near) null (see text for details) 
		for(int i=0;i<3; i++) 
			for(int j=0;j<3; j++) 
				AbsR[i][j] = abs(R[i][j]) + EPSILION;
		// Test axesL=A0,L=A1,L=A2 
		for(int i=0;i<3; i++) 
		{ 
			ra = e[i]; 
			rb = b.e[0] * AbsR[i][0] + b.e[1] * AbsR[i][1] + b.e[2] * AbsR[i][2]; 
			if (abs(t[i]) > ra + rb) return false;
		}

		// Test axisL=A0xB0 
		ra = e[1] * AbsR[2][0] + e[2] * AbsR[1][0]; 
		rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1]; 
		if (abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

		// Test axisL=A0xB1 
		ra = e[1] * AbsR[2][1] + e[2] * AbsR[1][1]; 
		rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0]; 
		if (abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;
		
		// Test axisL=A0xB2 
		ra = e[1] * AbsR[2][2] + e[2] * AbsR[1][2]; 
		rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0]; 
		if (abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;
		
		// Test axisL=A1xB0 
		ra = e[0] * AbsR[2][0] + e[2] * AbsR[0][0];
		rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
		if (abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;
		// Test axisL=A1xB1 
		ra = e[0] * AbsR[2][1] + e[2] * AbsR[0][1]; 
		rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0]; 
		if (abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

		// Test axisL=A1xB2 
		ra = e[0] * AbsR[2][2] + e[2] * AbsR[0][2]; 
		rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0]; 
		if (abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

		// Test axisL=A2xB0 
		ra = e[0] * AbsR[1][0] + e[1] * AbsR[0][0];
		rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
		if (abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

		// Test axisL=A2xB1 
		ra = e[0] * AbsR[1][1] + e[1] * AbsR[0][1]; 
		rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0]; 
		if (abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;
		
		// Test axisL=A2xB2 
		ra = e[0] * AbsR[1][2] + e[1] * AbsR[0][2]; 
		rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0]; 
		if (abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;
		// Since no separating axis is found, the OBBs must be intersecting
		return true;
	}
};