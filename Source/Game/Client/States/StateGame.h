#pragma once
#include "../../../Framework/GameState.h"
#include "../../Shared/Game.h"
#include "../../../Utility/Timer.h"

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
		m_game.update(dt);
		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		Timer t;
		t.startWatch();
		m_game.draw(draw);
		t.stopWatch();

		draw.getUiCam().apply(draw);
		Font& f = draw.getFont(Font::Style::Text, Font::Size::M);

		f.setColor(Color::White());
		f.write(draw, "draw time: " + std::to_string(t.getTimeMilli()), { 10,10 });
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

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		GameState::mouseMove(mpos, mdiff, handled);
		return true;
	}

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseDown(mpos, button);
		return true;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseUp(mpos, button);
		return true;
	}

	virtual bool wheel(const PointF& mpos, float amount) override
	{
		GameState::wheel(mpos, amount);
		return true;
	}

private:
	Game m_game;
};
