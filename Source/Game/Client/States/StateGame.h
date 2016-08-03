#pragma once
#include "../../../Framework/GameState.h"
#include "../../Shared/Game.h"

class StateGame : public GameState
{
public:
	StateGame()
	{
		m_game.registerMe(this);
	}
	virtual ~StateGame()
	{}

	virtual bool update(float dt) override 
	{
		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_game.draw(draw);

		draw.getUiCam().apply(draw);
		Font& f = draw.getFont(Font::Style::Text, Font::Size::M);

		f.setColor(Color::White());
		f.write("hello", { 10,10 });
	}

	// Input handling
	virtual bool keyDown(SDL_Scancode s) override
	{
		GameState::keyDown(s);
		return true;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		GameState::keyUp(s);
		return true;
	}

	virtual bool charDown(char c) override
	{
		GameState::charDown(c);
		return true;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		GameState::mouseMove(mpos, handled);
		return true;
	}

	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		GameState::mouseDown(button, mpos);
		return true;
	}

	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		GameState::mouseUp(button, mpos);
		return true;
	}

	virtual bool wheel(float amount, const PointF& mpos) override
	{
		GameState::wheel(amount, mpos);
		return true;
	}

private:
	Game m_game;
};
