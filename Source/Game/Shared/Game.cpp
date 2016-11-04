#include "Game.h"
#include "../../Framework/Color.h"
#include "MapLoader.h"

static float s = 0.5f;

std::unique_ptr<Cube> getCube(float y)
{
	return std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Random().toDWORD()), glm::vec3(0.0f, y, 0.0f), false, s));
}

Game::Game()
	:
	m_testNode(glm::vec3(5.0f, 5.0f, 0.0f))
{
	// load sample scene
	std::string filename = "sample_scene";
	MapLoader l(filename);
	if (!l.isOpen())
		throw Exception("cannot find " + filename);
	
	auto& i = l.getInfo();
	m_pMap = std::unique_ptr<Map>(new Map(Point3S(i.nChunks.x * MapChunk::SIZE, i.nChunks.y * MapChunk::SIZE, Map::DEPTH)));
	// TODO improve
	size_t idx = 0;
	for(const auto& chunk : i.chunkCubes)
	{
		Point3S off = Point3S(idx % i.nChunks.x, idx / i.nChunks.x, 0);
		off *= MapChunk::SIZE;
		for (const auto& c : chunk)
			m_pMap->setCube(c.first + off, c.second);
		
		idx++;
	}

	m_pCam = makeCamera();

	m_pLight = std::unique_ptr<LightManager>(new LightManager(*m_pCam));

	m_pLight->init(i.ambient, i.lights);

	auto nodeArmLeft = std::unique_ptr <CharNode>(new CharNode(glm::vec3(1.0f, -2.0f, 0.0f)));
	auto nodeArmRight = std::unique_ptr <CharNode>(new CharNode(glm::vec3(-1.0f, -2.0f, 0.0f)));

	std::vector<std::unique_ptr<Cube>> c;
	for (float y = 0; y < 5 * s; y += s)
	{
		c.push_back(getCube(y));
	}
	std::vector<std::unique_ptr<Cube>> la;
	for (float y = 0; y < 4 * s; y += s)
	{
		la.push_back(getCube(y));
	}
	std::vector<std::unique_ptr<Cube>> ra;
	for (float y = 0; y < 4 * s; y += s)
	{
		ra.push_back(getCube(y));
	}

	m_testNode.setCubes(std::move(c));
	nodeArmLeft->setCubes(std::move(la));
	nodeArmRight->setCubes(std::move(ra));

	nodeArmLeft->rotate(glm::rotate(-1.6f, glm::vec3(0.0f, 0.0f, 1.0f)));
	nodeArmRight->rotate(glm::rotate(0.8f, glm::vec3(0.0f, 0.0f, 1.0f)));

	m_testNode.addNode(std::move(nodeArmLeft));
	m_testNode.addNode(std::move(nodeArmRight));
}

Game::~Game()
{
}

void Game::update(float dt)
{
	m_char.update(dt);
}

#ifdef _CLIENT
void Game::draw(Drawing& draw)
{
	m_pCam->apply(draw);
	m_pLight->apply(draw);
	m_pMap->draw(draw);
	m_testNode.draw(draw);
	m_char.draw(draw);
	draw.getUiCam().apply(draw);
}

bool Game::keyDown(SDL_Scancode s)
{
	float d = 0.5f;
	PointF lookAt = m_pCam->getLookAt();

	switch (s)
	{
	case SDL_SCANCODE_LEFT:
		lookAt.x -= d;
		break;
	case SDL_SCANCODE_RIGHT:
		lookAt.x += d;
		break;
	case SDL_SCANCODE_UP:
		lookAt.y -= d;
		break;
	case SDL_SCANCODE_DOWN:
		lookAt.y += d;
		break;
	}

	m_pCam->setLookAt(lookAt);
	return true;
}

std::unique_ptr<Camera> Game::makeCamera()
{
	return std::unique_ptr<Camera>(new Camera({ 24.5f, 15.0f }, 30.0f, 70.0f, 5.0f, false));
}
#endif // _CLIENT
