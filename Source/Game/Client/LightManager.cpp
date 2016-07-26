#include "LightManager.h"

LightManager::LightManager(const Camera& cam)
	:
	m_cam(cam),
	m_uniformLight(Drawing::getDraw().getLightUniform())
{
}

void LightManager::init(const Color & ambient, std::vector<UniformBlockLight::LightSource> lights)
{
	m_ambient = ambient;
	m_lights = std::move(lights);

}

void LightManager::apply()
{
	if(!m_isApplied)
	{
		m_uniformLight.updateLights(m_ambient.toVec3(),
			m_cam.getEye(), m_lights);
		m_isApplied = true;
	}
	// TODO else check for changes => apply only on changes
}
