#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Utility/Timer.h"
#include "../../Shared/GameEngine.h"

class StateGame : public GameState
{
public:
	StateGame()
	{
		if (!m_engine.loadLevel("sample_scene"))
			throw Exception("cannot load sample scene");
	}
	virtual ~StateGame()
	{}

	virtual bool update(float dt) override 
	{
#ifdef _DEBUG
		m_engine.update(std::min(dt, 0.016f));
#else
		m_engine.update(dt);
#endif
		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		Timer t;
		Timer t2;
#ifdef _DEBUG
		glFinish();
#endif
		t.startWatch();

		draw.beginGameShader();
		t2.startWatch();
		m_engine.draw(draw);
#ifdef _DEBUG
		glFinish();
#endif
		t2.stopWatch();
		draw.endGameShader();

#ifdef _DEBUG
		glFinish();
#endif // snyc
		
		t.stopWatch();
	
		draw.getUiCam().apply(draw);
		Font& f = draw.getFont(Font::Style::Text, Font::Size::M);

		m_maxDrawTime = std::max(m_maxDrawTime, t.getTimeMilli());
		f.setColor(Color::White());
		f.write(draw, "draw time: " + std::to_string(t.getTimeMilli()), { 10,10 });
		f.write(draw, "draw max: " + std::to_string(m_maxDrawTime), { 10,50 });
		f.write(draw, "draw without fbo: " + std::to_string(t2.getTimeMilli()), { 10,90 });
	}

	// Input handling
	virtual bool keyDown(SDL_Scancode s) override
	{
		GameState::keyDown(s);
		return true;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		if (s == SDL_SCANCODE_R)
			m_maxDrawTime = 0.0f;
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
	GameEngine m_engine;
	float m_maxDrawTime = 0.0f;
};
