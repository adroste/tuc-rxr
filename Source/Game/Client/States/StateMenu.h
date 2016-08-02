#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/Sound/Sound.h"
#include "../../../Framework/UI/UIButton.h"
#include "StateGame.h"
<<<<<<< .mine
#include "../../../Framework/UI/UIButtonText.h"
#include "../../../Framework/OpenGL/Shader/UIColorPicker.h"
=======
#include "../../../Framework/UI/UIInputField.h"

>>>>>>> .theirs

class StateMenu : public GameState
{
public:
	StateMenu()
		:
		GameState(GameState::TransitionState::ForcePreserve),
<<<<<<< .mine
		m_cube(CubeDesc(Color::Red().toDWORD()), { 500.0f,500.0f,0.0f }, 70.0f),
		m_myButt(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::L), "Can't Touch This"),
		m_colorPicker(300, { 640, 360 })
=======
		m_cube(CubeDesc(Color::Red().toDWORD()),{500.0f,500.0f,0.0f},70.0f),
		m_inp(Drawing::getFont(Font::Style::Text,Font::Size::M),20)

>>>>>>> .theirs
	{
		//m_myButt.setDim({ 350, 80 });
		m_myButt.adjustToFontHeadline();
		m_myButt.center();

		m_inp.setDim({ 300,60 });
		m_inp.setOrigin({ 10,10 });

		m_myButt.registerMe(this);
		m_inp.registerMe(this);

		m_mpos = { 500,500 };
		Sound::playMusic(Sound::Music::Hey);
	}
	virtual ~StateMenu()
	{}

	virtual bool update(float dt) override
	{
		if (m_myButt.isClicked(true))
		{	
			Sound::playMusic(Sound::Music::Theme);
			//Sound::playSound(Sound::Sfx::Plop);
			setNextState(std::unique_ptr<GameState>(new StateGame()));
		}

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		//m_myButt.draw(draw);
		//m_colorPicker.draw(draw);
<<<<<<< .mine

		draw.line({ 640, 360 }, m_mpos, 100.0f, Color::Cyan());

		draw.rect(RectF::constructFromPoint(m_mpos, 10.0f), Color::Red());


		draw.getFont(Font::Style::Text, Font::Size::L).setColor(Color::White());
		draw.getFont(Font::Style::Text, Font::Size::L).write("Sample", { 100,100 });
=======
		m_inp.draw(draw);







>>>>>>> .theirs
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		handled = GameState::mouseMove(mpos, handled);

		m_cube.setRot(m_cube.getRot() + glm::vec3(0.01f, 0.02f, 0.03f));

		m_mpos = mpos;
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
	UIButtonText m_myButt;
	UIInputField m_inp;

	Cube m_cube;

	UIColorPicker m_colorPicker;
};
