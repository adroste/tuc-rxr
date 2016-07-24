#include "Game.h"
#include "../../Framework/Color.h"

Game::Game()
	:
	m_testNode(glm::vec3(5.0f, 5.0f, 0.0f))
{
	m_pMap = std::unique_ptr<Map>(new Map(50, 30, 4));
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

	for (size_t x = 0; x < dim.width; ++x)
	{
		for (size_t z = 0; z < dim.depth; ++z)
		{
			m_pMap->setCube(new Cube(CubeDesc(Color::Green().toDWORD()), glm::vec3(x, m_pMap->getDim().height - 1, z)));
		
			if (x > 10 && x < 40)
				m_pMap->setCube(new Cube(CubeDesc(Color::Red().toDWORD()), glm::vec3(x, 10, z)));
		}
	}

#ifdef _CLIENT
	m_pCam = std::unique_ptr<Camera>(new Camera({ 24.5f, 15.0f }, 30.0f, 30.0f, 5.0f));
#endif // _CLIENT

	auto nodeArmLeft = std::unique_ptr <CharNode>(new CharNode(glm::vec3(1.0f, -2.0f, 0.0f)));
	auto nodeArmRight = std::unique_ptr <CharNode>(new CharNode(glm::vec3(-1.0f, -2.0f, 0.0f)));

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
