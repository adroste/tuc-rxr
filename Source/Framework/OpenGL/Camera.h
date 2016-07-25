#pragma once
#include "../../glm/gtx/transform.hpp"
#include "../../Utility/Point.h"

class Camera
{
public:
	Camera(const PointF& lookAt, float height, float dist, float heightOffset = 0.0f, bool overrideEqualDepth = true);
	virtual ~Camera();

	void apply(class Drawing& draw) const;

	const PointF& getLookAt() const;
	void setLookAt(const PointF& lookAt);


	float getHeight() const;
	void setHeight(const float height);
	float getDist() const;
	void setDist(const float dist);

private:
	void recalcProject();
	void recalcCam();

private:
	glm::mat4 m_matProject;
	glm::mat4 m_matCam;

	PointF m_lookAt;
	float m_height;
	float m_dist;
	float m_heightOffset;
	bool m_overrideEqualDepth;
};
