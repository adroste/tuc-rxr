#include "GameEditor.h"
#include "../Shared/Game.h"
#include "../../System/System.h"

GameEditor::GameEditor()
	:
	m_prevClientMouse(Input::getMouse()),
	m_curCubeDesc(Color::Green().toDWORD())
{
	m_pMap = std::unique_ptr<Map>(new Map({ 10,8,4 }));
	m_pMap->setCube(new Cube(CubeDesc(Color::Green().toDWORD()),{3,3,3},true));

	m_pCam = Game::makeCamera();
	m_pLight = std::unique_ptr<LightManager>(new LightManager(*m_pCam));

	// add light sources
	std::vector<UniformBlockLight::LightSource> lights;
	UniformBlockLight::LightSource l;
	l.type = UniformBlockLight::LightSource::Directional;
	l.color = Color::White().toVec3();
	l.origin = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	lights.push_back(l);

	l.type = UniformBlockLight::LightSource::PointLight;
	l.color = Color::White().toVec3();
	l.origin = { 10,10,-10 };//glm::normalize(glm::vec3(0.2f, -1.0f, 0.3f));
	l.attenuation = 0.00001f;
	lights.push_back(l);

	m_pLight->init(Color::Gray(0.01f), std::move(lights));

	m_pCam->setLookAt({5,4});
}

GameEditor::~GameEditor()
{
	releaseCapture();
}

void GameEditor::draw(Drawing& draw)
{
	m_pCam->apply(draw);
	m_pLight->apply(draw);
	m_pMap->draw(draw);

	drawGrid(draw);
	if(m_hasCapture)
	{
		// draw block in mouse coordinates
		Cube curCube = Cube(m_curCubeDesc,m_blockPos.toGlmVec3(),false);
		curCube.draw(draw);
		
		//draw.rect(RectF::constructFromPoint({ m_blockPos.x,m_blockPos.z }, 1.0f), Color::Red());
	}

	draw.getUiCam().apply(draw);

	// draw debug position
	auto& f = draw.getFont(Font::Style::Text, Font::Size::S);
	f.write(draw, std::to_string(m_blockPos.x)
		+ " : " + std::to_string(m_blockPos.y)
		+ " : " + std::to_string(m_blockPos.z), { 10,10 });
}

bool GameEditor::mouseMove(const PointF& mpos, bool handled)
{
	m_mouseDiff -= (m_prevClientMouse - mpos) * 0.1f;
	m_prevClientMouse = mpos;

	if(m_hasCapture)
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
			// set block
			m_pMap->setCube(new Cube(m_curCubeDesc, m_blockPos.toGlmVec3(), true), false, true);
		}
		else if (m_hover) // && !m_capture
			takeCapture();
		break;
	case Input::Mouse::Middle: break;
	case Input::Mouse::Right: break;
	case Input::Mouse::X1: break;
	case Input::Mouse::X2: break;
	}

	return true;
}

bool GameEditor::mouseUp(const PointF& mpos, Input::Mouse button)
{
	switch (button)
	{
	case Input::Mouse::Left: break;
	case Input::Mouse::Middle: break;
	case Input::Mouse::Right: break;
	case Input::Mouse::X1: break;
	case Input::Mouse::X2: break;
	default: break;
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

void GameEditor::setCurrentBlock(const CubeDesc& c)
{
	m_curCubeDesc = c;
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

void GameEditor::releaseCapture()
{
	m_hasCapture = false;
	System::showCursor();
	System::setTrapCursor(false);
}

void GameEditor::takeCapture()
{
	m_hasCapture = true;
	System::hideCursor();
	System::setTrapCursor(true);
}
