#pragma once
#include "../Framework/OpenGL/Camera.h"
#include "../Framework/OpenGL/Shader/UniformBlockLight.h"
#include "../Framework/OpenGL/Drawing.h"
#include <vector>

class LightManager
{
public:
	LightManager(const Camera& cam);
	void init(const Color& ambient ,std::vector<LightSource> lights);
	void apply(Drawing& draw);

	const Color& getAmbient() const;
	const std::vector<LightSource>& getLights() const;
private:
	const Camera& m_cam;
	std::vector<LightSource> m_lights;
	Color m_ambient;
	bool m_isApplied = false;
};