#include "Camera.h"

#include <math.h>
#include "../Framework.h"
#include "Drawing.h"

Camera::Camera(const PointF& lookAt, float height, float dist, float heightOffset, bool overrideEverything)
	:
	m_lookAt(lookAt),
	m_height(height),
	m_dist(dist),
	m_heightOffset(heightOffset),
	m_overrideEverything(overrideEverything)
{
	recalcProject();
	recalcCam();
}

Camera::~Camera()
{

}

void Camera::apply(Drawing& draw)
{
	if (m_overrideEverything)
		glDepthFunc(GL_ALWAYS);
	else
		glDepthFunc(GL_LESS);

	// check for aspect change
	if (m_lastAspect != Framework::getAspect())
		recalcProject();

	draw.getTransform().setProjection(m_matProject);
	draw.getTransform().setCamera(m_matCam);
}

void Camera::recalcProject()
{
	float fovY = 2.0f * atanf(m_height / (2.0f * m_dist));
	m_lastAspect = Framework::getAspect();
	m_matProject = glm::perspective(fovY, m_lastAspect, m_dist / 1.5f, m_dist * 1.5f);
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
	if (m_lookAt == lookAt) return;
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
