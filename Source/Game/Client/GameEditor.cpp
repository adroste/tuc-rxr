#include "GameEditor.h"
#include "../Shared/Game.h"
#include "../../System/System.h"
#include <functional>

GameEditor::GameEditor()
	:
	m_prevClientMouse(Input::getMouse())
{
	reset();
}

GameEditor::~GameEditor()
{
	releaseCapture();
}

void GameEditor::draw(Drawing& draw, float dt)
{
	LockGuard g(m_muMap);

	m_pCam->apply(draw);
	m_pLight->apply(draw);
	m_pMap->draw(draw);

	drawGrid(draw);
	if(m_hasCapture)
	{
		const Point3F fpos = m_blockPos;
		const Point3F off = Point3F(0.5f, 0.5f, 0.5f);
		// draw block in mouse coordinates
		glDepthFunc(GL_LEQUAL);
		if(hasBlock())
		{
			static size_t divider = 1;
			static float sumdt = 0.0f;
			sumdt += dt;
			if (sumdt > 2.0f)
			{
				sumdt -= 2.0f;
				divider++;
			}

			Cube curCube = Cube(m_curCubeDescs[divider % m_curCubeDescs.size()], m_blockPos.toGlmVec3(), false);
			curCube.draw(draw);
		}

		glDepthFunc(GL_ALWAYS);
		drawLineBox(draw, fpos - off, fpos + off, Color::Gray());

		
		if (m_setDown || m_eraseDown)
		{
			drawLineBox(draw, 
				Point3F(std::min(m_blockPos, m_downPos)) - off,
				Point3F(std::max(m_blockPos, m_downPos)) + off,
				Color::Red());
		}

	}

	draw.getUiCam().apply(draw);

	// draw debug position
	auto& f = draw.getFont(Font::Style::Text, Font::Size::S);
	f.write(draw, std::to_string(m_blockPos.x)
		+ " : " + std::to_string(m_blockPos.y)
		+ " : " + std::to_string(m_blockPos.z), { 10,300 });
}

bool GameEditor::mouseMove(const PointF& mpos, const PointF& mdiff, bool handled)
{
	if(m_hasCapture)
	{
		//m_mouseDiff -= (m_mouseCapPos - mpos) * 0.025f;
		//Input::setCamMouse(m_mouseCapPos);
		m_mouseDiff += mdiff * 0.05f;

		if (!m_dragDown)
		{
			auto xdiff = int(m_mouseDiff.x);
			auto ydiff = int(m_mouseDiff.y);

			m_blockPos.x += xdiff;
			m_blockPos.y += ydiff;

			m_mouseDiff.x -= float(xdiff);
			m_mouseDiff.y -= float(ydiff);

			// clip
			m_blockPos.x = tool::clamp(m_blockPos.x, 0, signed(m_pMap->getDim().x - 1));
			m_blockPos.y = tool::clamp(m_blockPos.y, 0, signed(m_pMap->getDim().y - 1));
		}
		else
		{
			// dragging
			m_pCam->setLookAt(m_pCam->getLookAt() - m_mouseDiff);
			m_mouseDiff = PointF(0.0f, 0.0f);
		}
	}

	m_hover = !handled;

	return true;
}

bool GameEditor::mouseDown(const PointF& mpos, Input::Mouse button)
{
	switch (button)
	{
	case Input::Mouse::Left: 
		if(m_hasCapture)
		{
			if (!m_eraseDown && !m_dragDown)
			{
				m_setDown = true;
				m_downPos = m_blockPos;
			}
			// set block
			/*m_pMap->setCube(new Cube(m_curCubeDesc, m_blockPos.toGlmVec3(), true), false, true);*/
		}
		else if (m_hover) // && !m_capture
			takeCapture();
		break;
	case Input::Mouse::Middle:
		if (!m_hasCapture && m_hover)
		{
			m_isCaptureTemp = true;
			takeCapture();
		}
		if(m_hasCapture && !m_setDown && !m_eraseDown)
		{
			m_dragDown = true;
			m_mouseDiff = PointF(0.0f, 0.0f);
		}
		break;
	case Input::Mouse::Right: 
		if(!m_setDown && m_hasCapture && !m_dragDown)
		{
			m_eraseDown = true;
			m_downPos = m_blockPos;
		}
		break;
	case Input::Mouse::X1: break;
	case Input::Mouse::X2: break;
	}

	return true;
}

bool GameEditor::mouseUp(const PointF& mpos, Input::Mouse button)
{
	if(m_hasCapture)
	{
		auto forAllInBox = [](Point3I start, Point3I end, std::function<void(Point3I)> doFunc)
		{
			Point3S s = std::min(start,end);
			Point3S e = std::max(start,end);

			for (auto x = s.x; x <= e.x; x++)
				for (auto y = s.y; y <= e.y; y++)
					for (auto z = s.z; z <= e.z; z++)
						doFunc({ x,y,z });
		};
		                                                                 
		switch (button)
		{
		case Input::Mouse::Left: 
			if(m_setDown)
			{
				m_setDown = false;
				if(hasBlock())
				{
					forAllInBox(m_downPos, m_blockPos, [this](Point3S p)
					{
						m_pMap->setCube(new Cube(getNextBlock(), p.toGlmVec3(), true), false, true);
					});
				}
			}
			break;
		case Input::Mouse::Middle: 
			if(m_dragDown)
			{
				m_dragDown = 0.0f;
			}
			break;
		case Input::Mouse::Right:
			// delete block
			if(m_eraseDown)
			{
				m_eraseDown = false;
				forAllInBox(m_downPos, m_blockPos, [this](Point3S p)
				{
					m_pMap->destroyBlock(p);
				});
			}
			m_pMap->destroyBlock(m_blockPos);
			break;
		case Input::Mouse::X1: break;
		case Input::Mouse::X2: break;
		default: break;
		}

		if (m_isCaptureTemp)
		{
			m_isCaptureTemp = false;
			releaseCapture();
		}
	}
	return true;
}

bool GameEditor::wheel(const PointF& mpos, float amount)
{
	if(m_hasCapture)
	{
		m_wheelDiff += amount;
		auto zdiff = int(m_wheelDiff);

		m_blockPos.z += zdiff;
		m_wheelDiff -= float(zdiff);

		m_blockPos.z = tool::clamp(m_blockPos.z, 0, signed(m_pMap->getDim().z - 1));
	}

	return true;
}

bool GameEditor::keyDown(SDL_Scancode s)
{
	switch (s)
	{
	case SDL_SCANCODE_LEFT:
		if(m_pMap->getDim().x > 1)
			m_pMap->setDim(m_pMap->getDim() - Point3S(1, 0, 0));
		break;
	case SDL_SCANCODE_RIGHT:
		m_pMap->setDim(m_pMap->getDim() + Point3S(1, 0, 0));
		break;
	case SDL_SCANCODE_UP:
		m_pMap->setDim(m_pMap->getDim() + Point3S(0, 1, 0));
		break;
	case SDL_SCANCODE_DOWN:
		if (m_pMap->getDim().y > 1)
			m_pMap->setDim(m_pMap->getDim() - Point3S(0, 1, 0));
		break;

	}
	return true;
}

bool GameEditor::keyUp(SDL_Scancode s)
{
	switch (s)
	{
	case SDL_SCANCODE_ESCAPE:
		releaseCapture();
		break;
	}
	return true;
}

void GameEditor::setCurrentBlocks(std::vector<CubeDesc> v)
{
	m_curCubeDescs = std::move(v);
}

void GameEditor::setMapdim(const Point3S& d)
{
	m_pMap->setDim(d);
}

void GameEditor::reset()
{
	LockGuard g(m_muMap);
	m_pMap = std::unique_ptr<Map>(new Map({ 10,8,4 }));
	m_pCam = Game::makeCamera();

	m_pLight = std::unique_ptr<LightManager>(new LightManager(*m_pCam));

	// add light sources
	std::vector<LightSource> lights;
	LightSource l;
	l.type = LightSource::Directional;
	l.color = Color::White().toVec3();
	l.origin = glm::normalize(glm::vec3(0.0f, 1.0f, 0.5f));
	lights.push_back(l);

	m_pLight->init(Color::Gray(0.01f), std::move(lights));

	m_pCam->setLookAt({ 5,4 });
}

const Color& GameEditor::getAmbientColor() const
{
	return m_pLight->getAmbient();
}

const std::vector<LightSource>& GameEditor::getLights() const
{
	return m_pLight->getLights();
}

void GameEditor::updateLights(const Color& ambient, const std::vector<LightSource>& lights)
{
	m_pLight->init(ambient, lights);
}

Point3S GameEditor::getMapSize() const
{
	return m_pMap->getDim();
}

std::vector<std::pair<CubeDesc, Point3S>> GameEditor::getCubeDescs()
{
	return m_pMap->getCubeInfos();
}

void GameEditor::loadCubes(const std::vector<std::pair<CubeDesc, Point3S>>& c)
{
	for(const auto& o : c)
	{
		m_pMap->setCube(new Cube(o.first, o.second.toGlmVec3(), true),false,true);
	}
}

void GameEditor::drawGrid(Drawing& draw) const
{
	draw.getTransform().pushModel(glm::translate(glm::vec3{ -0.5f, -0.5f, -0.5f }));
	// draw background
	auto dim = m_pMap->getDim();
	for (size_t z = 0; z <= dim.z; z++)
		draw.line(
			glm::vec3(0.0f, float(dim.y), float(z)),
			glm::vec3(float(dim.x), float(dim.y), float(z)),
			1.0f, Color::Blue()
		);

	for (size_t y = 0; y < dim.y; y++)
		draw.line(
			glm::vec3(0.0f, float(y), float(dim.z)),
			glm::vec3(float(dim.x), float(y), float(dim.z)),
			1.0f, Color::Blue()
		);

	for (size_t x = 0; x <= dim.x; x++)
	{
		float fx = float(x);
		draw.line(
			glm::vec3(fx, 0.0f, float(dim.z)),
			glm::vec3(fx, float(dim.y), float(dim.z)),
			1.0f, Color::Blue()
		);
		draw.line(
			glm::vec3(fx, float(dim.y), 0.0f),
			glm::vec3(fx, float(dim.y), float(dim.z)),
			1.0f, Color::Blue()
		);
	}

	draw.getTransform().popModel();
}

void GameEditor::drawLineBox(Drawing& draw, const Point3F& p1, const Point3F& p2, const Color& c) const
{
	auto s = p1.toGlmVec3();//std::min(p1, p2).toGlmVec3();
	auto e = p2.toGlmVec3();//std::max(p1, p2).toGlmVec3();
	float t = 1.0f;

	draw.line(s, glm::vec3(s.x,s.y,e.z), t, c);
	draw.line(s, glm::vec3(s.x, e.y, s.z), t, c);
	draw.line(s, glm::vec3(e.x, s.y, s.z), t, c);
	draw.line(e, glm::vec3(e.x, e.y, s.z), t, c);
	draw.line(e, glm::vec3(e.x, s.y, e.z), t, c);
	draw.line(e, glm::vec3(s.x, e.y, e.z), t, c);
	
	draw.line(glm::vec3(s.x, e.y, s.z), glm::vec3(s.x, e.y, e.z), t, c);
	draw.line(glm::vec3(e.x, s.y, s.z), glm::vec3(e.x, s.y, e.z), t, c);
	
	draw.line(glm::vec3(s.x, s.y, e.z), glm::vec3(e.x, s.y, e.z), t, c);
	draw.line(glm::vec3(s.x, e.y, s.z), glm::vec3(e.x, e.y, s.z), t, c);

	draw.line(glm::vec3(s.x, s.y, e.z), glm::vec3(s.x, e.y, e.z), t, c);
	draw.line(glm::vec3(e.x, s.y, s.z), glm::vec3(e.x, e.y, s.z), t, c);
}

void GameEditor::releaseCapture()
{
	m_hasCapture = false;
	Input::freeMouse();
	m_onCapture(false);
	m_dragDown = false;
	m_setDown = false;
	m_eraseDown = false;
}

void GameEditor::takeCapture()
{
	m_hasCapture = true;
	//System::hideCursor();
	//System::setTrapCursor(true);
	Input::trapMouse();
	m_onCapture(true);
}

const CubeDesc& GameEditor::getNextBlock() const
{
	assert(hasBlock());
	return m_curCubeDescs[rand() % m_curCubeDescs.size()];
}

bool GameEditor::hasBlock() const
{
	return m_curCubeDescs.size() != 0;
}
