#include "Camera.h"

#include <math.h>
#include "../Framework.h"

Camera::Camera(const PointF & lookAt, float height, float dist)
	:
	m_lookAt(lookAt),
	m_height(height),
	m_dist(dist)
{
	recalcProject();
	recalcCam();
}

Camera::~Camera()
{

}

void Camera::apply(Drawing& draw) const
{
	draw.setProjection(m_matProject);
	draw.setCamera(m_matCam);
}

void Camera::recalcProject()
{
	float fovY = 2.0f * atanf(m_height / (2.0f * m_dist));
	m_matProject = glm::perspective(fovY, Framework::getAspect(), m_dist / 1.5f, m_dist * 1.5f);
}

void Camera::recalcCam()
{
	m_matCam = glm::lookAt(
		glm::vec3(m_lookAt.x, m_lookAt.y, -m_dist), // camera pos
		glm::vec3(m_lookAt.x, m_lookAt.y, 0.0f), // lookAt
		glm::vec3(0.0f, -1.0f, 0.0f)); // up vec
}

const PointF& Camera::getLookAt() const
{
	return m_lookAt;
}

void Camera::setLookAt(const PointF& lookAt)
{
	m_lookAt = lookAt;
	recalcCam();
}
