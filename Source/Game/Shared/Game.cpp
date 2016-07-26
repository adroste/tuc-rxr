#include "Game.h"
#include "../../Framework/Color.h"

static float s = 0.5f;
std::unique_ptr<Cube> getCube(float y)
{
	return std::unique_ptr<Cube>(new Cube(CubeDesc(Color::Random().toDWORD()), glm::vec3(0.0f, y, 0.0f), s));
}

CubeDesc getStoneDesc()
{
	return CubeDesc(Color().Gray(0.45f + ((rand() % 256) / 256.0f) * 0.4f).toDWORD());
}
CubeDesc getDirtStone()
{
	if (rand() % 3 == 0)
		return getStoneDesc();
	return CubeDesc(Color::Brown().toDWORD());
}
CubeDesc getGrassDesc()
{
	return  CubeDesc(Color::Green().toDWORD());
}

void loadCaveScene(std::unique_ptr<Map>& m)
{
	// floor
	m = std::unique_ptr<Map>(new Map(Point3S(50, 35, 4)));
	Point3S d = m->getDim();
	for(size_t x = 0; x < d.width; x++)
	{
		for(size_t z = 0; z < d.depth; z++)
		{
			m->setCube(new Cube(getStoneDesc(), glm::vec3(x, d.height - 1, z)));
		}
	}

	// cave wall
	for (size_t x = 0; x < d.width; x++)
	{
		for (size_t y = d.height - 6; y < d.height - 1; y++)
		{
			m->setCube(new Cube(getStoneDesc(), glm::vec3(x, y, d.depth - 1)));
		}
	}

	// dirt
	for (size_t x = 0; x < d.width; x++)
	{
		for (size_t z = 0; z < d.depth; z++)
		{
			for(size_t y = d.height - 8; y < d.height - 6; y++)
			m->setCube(new Cube(getDirtStone(), glm::vec3(x, y, z)));
		}
	}

	// grass
	for (size_t x = 0; x < d.width; x++)
	{
		for (size_t z = 0; z < d.depth; z++)
		{
			m->setCube(new Cube(getGrassDesc(), glm::vec3(x, d.height - 9, z)));
		}
	}
}

Game::Game()
	:
	m_testNode(glm::vec3(5.0f, 5.0f, 0.0f))
{
	//m_pMap = std::unique_ptr<Map>(new Map(50, 30, 4));
	loadCaveScene(m_pMap);
	Point3S dim = m_pMap->getDim();
	//for (size_t x = 0; x < dim.width; x += 2)
	//{
	//	for (size_t y = 0; y < dim.height; y += 2)
	//	{
	//		for (size_t z = 0; z < dim.depth; z += 2)
	//		{
	//			float rx = (rand() % 256) / 256.0f;
	//			float ry = (rand() % 256) / 256.0f;
	//			float rz = (rand() % 256) / 256.0f;
	//			m_pMap->setCube(new Cube(CubeDesc(Color(rx,ry,rz).toDWORD()), glm::vec3(x, y, z)));
	//		}
	//	}

	//	for (size_t z = 1; z < dim.depth; ++z)
	//	{
	//		m_pMap->setCube(new Cube(CubeDesc(Color::Green().toDWORD()), glm::vec3(x, 9, z)));
	//	}
	//}

	

	/*for (size_t x = 0; x < dim.width; ++x)
	{
		for (size_t z = 0; z < dim.depth; ++z)
		{
			m_pMap->setCube(new Cube(CubeDesc(Color::Green().toDWORD()), glm::vec3(x, m_pMap->getDim().height - 1, z)));
		
			if (x > 10 && x < 40)
				m_pMap->setCube(new Cube(CubeDesc(Color::Red().toDWORD()), glm::vec3(x, 10, z)));
		}
	}*/

#ifdef _CLIENT
	m_pCam = std::unique_ptr<Camera>(new Camera({ 24.5f, 15.0f }, 30.0f, 30.0f, 5.0f, false));

	m_pLight = std::unique_ptr<LightManager>(new LightManager(*m_pCam));

	// add light sources
	std::vector<UniformBlockLight::LightSource> lights;
	UniformBlockLight::LightSource l;
	l.type = UniformBlockLight::LightSource::Directional;
	l.color = (Color::White() * 0.4f).toVec3();
	l.origin = glm::normalize(glm::vec3(0.1f, 1.0f, 0.7f));
	//lights.push_back(l);

	l.type = UniformBlockLight::LightSource::PointLight;
	l.color = Color(1.0f, 0.8f, 0.4f).toVec3();
	l.origin = glm::vec3(9, 20, dim.depth / 2);
	l.attenuation = 0.01f;
	lights.push_back(l);
	m_pMap->setCube(new Cube(CubeDesc(Color::White().toDWORD()), l.origin, 0.5f), true);

	// torch
	l.color = Color(1.0f, 0.2f, 0.1f).toVec3();
	l.origin = glm::vec3(30, 31, dim.depth - 2);
	l.attenuation = 0.04f;
	lights.push_back(l);
	m_pMap->setCube(new Cube(CubeDesc(Color::Red().toDWORD()), l.origin, 0.5f), true);

	l.color = Color(0.0f, 1.0f, 0.0f).toVec3();
	l.origin = glm::vec3(25, 31, dim.depth - 2);
	lights.push_back(l);
	m_pMap->setCube(new Cube(CubeDesc(Color::Green().toDWORD()), l.origin, 0.5f), true);

	l.color = Color(0.0f, 0.0f, 1.0f).toVec3();
	l.origin = glm::vec3(20, 31, dim.depth - 2);
	lights.push_back(l);
	m_pMap->setCube(new Cube(CubeDesc(Color::Blue().toDWORD()), l.origin, 0.5f), true);

	m_pLight->init(Color(0.05f, 0.05f, 0.05f), std::move(lights));
#endif // _CLIENT

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


#ifdef _CLIENT
void Game::draw(Drawing& draw)
{
	m_pCam->apply(draw);
	m_pLight->apply();
	m_pMap->draw(draw);
	m_testNode.draw(draw);
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
#endif // _CLIENT
