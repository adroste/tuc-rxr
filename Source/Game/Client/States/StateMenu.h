#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/Sound/Sound.h"
#include "../../../Framework/UI/UIButton.h"

class StateMenu : public GameState
{
public:
	StateMenu()
		:
		GameState(GameState::TransitionState::ForcePreserve),
		m_cube(CubeDesc(),{500.0f,500.0f,0.0f},15.0f)
	{
		m_myButt.setDim({ 300, 100 });
		m_myButt.center();

		m_myButt.registerMe(this);

		m_mpos = { 500,500 };
		//Sound::playMusic(Sound::Music::Hey);
	}
	virtual ~StateMenu()
	{}

	virtual bool update(float dt) override
	{
		if (m_myButt.isClicked(true))
			Sound::playMusic(Sound::Music::Theme);
			//Sound::playSound(Sound::Sfx::Plop);

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_myButt.draw(draw);

		//draw.rect(RectF::constructFromPoint(m_mpos, 10.0f), Color::Red());

		/*draw.coloredCube(m_mpos, 15.0f, Color::Cyan());
		draw.coloredCube(m_mpos, 15.0f, Color::Cyan(), 30.0f);
		draw.coloredCube(m_mpos, 15.0f, Color::Cyan(), -30.0f);
		draw.coloredCube(m_mpos, 15.0f, Color::Cyan(), 60.0f);

		draw.coloredCube(m_mpos + PointF(30.0f,0.0f), 15.0f, Color::Cyan());*/
		
		m_cube.draw(draw);

		draw.getFont(Font::Style::Text, Font::Size::L).setColor(Color::White());
		draw.getFont(Font::Style::Text, Font::Size::L).write("Sample", { 100,100 });
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		handled = GameState::mouseMove(mpos, handled);

		m_cube.setRot(m_cube.getRot() + glm::vec3(0.1f, 0.2f, 0.0f));

		//m_mpos = mpos;
		return handled;
	}
	virtual bool keyDown(SDL_Scancode s) override
	{
		bool h = GameState::keyDown(s);

		switch(s)
		{
			case SDL_SCANCODE_LEFT:
				m_mpos.x--;
				break;
			case SDL_SCANCODE_RIGHT:
				m_mpos.x++;
				break;
			case SDL_SCANCODE_UP:
				m_mpos.y--;
				break;
			case SDL_SCANCODE_DOWN:
				m_mpos.y++;
				break;
		}

		m_cube.setPos({ m_mpos.x,m_mpos.y, 0.0f });

		return h;
	}
private:
	PointI m_mpos;
	UIButton m_myButt;

	Cube m_cube;
};
