#include "GameEditor.h"
#include "../Shared/Game.h"

GameEditor::GameEditor()
	:
	m_mapDim(10,8,4)
{
	m_pCam = Game::makeCamera();
	m_pLight = std::unique_ptr<LightManager>(new LightManager(*m_pCam));

	m_pCam->setLookAt({ 5,4 });
}

GameEditor::~GameEditor()
{
}

void GameEditor::draw(Drawing& draw)
{
	m_pCam->apply(draw);

	drawGrid(draw);



	draw.getUiCam().apply(draw);
}

void GameEditor::drawGrid(Drawing& draw) const
{
	// draw background
	for (size_t z = 0; z <= m_mapDim.z; z++)
		draw.line(
			glm::vec3(0.0f, float(m_mapDim.y), float(z)),
			glm::vec3(float(m_mapDim.x), float(m_mapDim.y), float(z)),
			1.0f, Color::Blue()
		);

	for (size_t y = 0; y < m_mapDim.y; y++)
		draw.line(
			glm::vec3(0.0f, float(y), float(m_mapDim.z)),
			glm::vec3(float(m_mapDim.x), float(y), float(m_mapDim.z)),
			1.0f,Color::Blue()
		);

	for(size_t x = 0; x <= m_mapDim.x; x++)
	{
		float fx = float(x);
		draw.line(
			glm::vec3(fx, 0.0f, float(m_mapDim.z)),
			glm::vec3(fx, float(m_mapDim.y), float(m_mapDim.z)),
			1.0f, Color::Blue()
		);
		draw.line(
			glm::vec3(fx, float(m_mapDim.y), 0.0f),
			glm::vec3(fx, float(m_mapDim.y), float(m_mapDim.z)),
			1.0f, Color::Blue()
		);
	}
}