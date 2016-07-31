#include "Camera.h"

#include <math.h>
#include "../Framework.h"
#include "Drawing.h"

Camera::Camera(const PointF& lookAt, float height, float dist, float heightOffset, bool overrideEqualDepth)
	:
	m_lookAt(lookAt),
	m_height(height),
	m_dist(dist),
	m_heightOffset(heightOffset),
	m_overrideEqualDepth(overrideEqualDepth)
{
	recalcProject();
	recalcCam();
}

Camera::~Camera()
{

}

void Camera::apply(Drawing& draw) const
{
	if (m_overrideEqualDepth)
		glDepthFunc(GL_LEQUAL);
	else
		glDepthFunc(GL_LESS);

	draw.setProjection(m_matProject);
	draw.setCamera(m_matCam);

	// TODO compatibilty reasons
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&m_matProject[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&m_matCam[0][0]);
}

void Camera::recalcProject()
{
	float fovY = 2.0f * atanf(m_height / (2.0f * m_dist));
	m_matProject = glm::perspective(fovY, Framework::getAspect(), m_dist / 1.5f, m_dist * 1.5f);
}

void Camera::recalcCam()
{
	m_matCam = glm::lookAt(
		getEye(), // camera pos
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

float Camera::getHeight() const
{
	return m_height;
}

void Camera::setHeight(const float height)
{
	m_height = height;
	recalcProject();
}

float Camera::getDist() const
{
	return m_dist;
}

void Camera::setDist(const float dist)
{
	m_dist = dist;
	recalcProject();
	recalcCam();
}

glm::vec3 Camera::getEye() const
{
	return glm::vec3(m_lookAt.x, m_lookAt.y - m_heightOffset, -m_dist);
}
