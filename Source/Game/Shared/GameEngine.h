#pragma once
#include "GameTypes.h"
#include "../../Framework/OpenGL/Drawing.h"
#include "Map.h"
#include "../Client/LightManager.h"

class GameEngine
{
public:
	GameEngine();
	virtual ~GameEngine() = default;
	bool loadLevel(const std::string& filename);
	virtual void update(float dt);
	virtual void draw(Drawing& draw);
private:
	GameManager m_manager;
	Map m_map;
	Camera m_cam;
	LightManager m_light;
};
