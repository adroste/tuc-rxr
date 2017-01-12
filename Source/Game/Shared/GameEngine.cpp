#include "GameEngine.h"
#include "MapLoader.h"

GameEngine::GameEngine()
	:
m_light(m_cam),
m_map(m_manager)
{
	// TODO add manager queries
	m_manager.start();
}

bool GameEngine::loadLevel(const std::string& filename)
{
	MapLoader l(filename);
	if (!l.isOpen())
		return false;

	auto& i = l.getInfo();

	m_map.setDim(Point3S(i.nChunks.x * MapChunk::SIZE,i.nChunks.y * MapChunk::SIZE, 16));
	m_map.loadMapAndAssets(i);

	m_cam = Camera({ 24.5f, 15.0f }, 30.0f, 70.0f, 5.0f, false);
	m_light.init(i.ambient, i.lights);

	return true;
}

void GameEngine::update(float dt)
{
	m_manager.tick(dt);
}

void GameEngine::draw(Drawing& draw)
{
	m_cam.apply(draw);

	m_light.apply(draw);
	m_map.draw(draw);
	draw.getUiCam().apply(draw);
}
