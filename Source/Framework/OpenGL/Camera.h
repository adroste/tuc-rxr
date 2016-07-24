#pragma once
#include "../../glm/gtx/transform.hpp"
#include "../../Utility/Point.h"
#include "Drawing.h"

class Camera
{
public:
	Camera(const PointF& lookAt, float height, float dist);
	virtual ~Camera();

	void apply(Drawing& draw) const;

	const PointF& getLookAt() const;
	void setLookAt(const PointF& lookAt);

private:
	void recalcProject();
	void recalcCam();

private:
	glm::mat4 m_matProject;
	glm::mat4 m_matCam;

	PointF m_lookAt;
	float m_height;
	float m_dist;
};
