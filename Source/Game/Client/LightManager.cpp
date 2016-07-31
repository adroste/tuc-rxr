#include "LightManager.h"

LightManager::LightManager(const Camera& cam)
	:
	m_cam(cam)
{
}

void LightManager::init(const Color & ambient, std::vector<UniformBlockLight::LightSource> lights)
{
	m_ambient = ambient;
	m_lights = std::move(lights);

}

void LightManager::apply(Drawing& draw)
{
	if(!m_isApplied)
	{
		draw.setLights(m_ambient, m_lights, m_cam.getEye());
		m_isApplied = true;
	}
	// TODO else check for changes => apply only on changes
}
