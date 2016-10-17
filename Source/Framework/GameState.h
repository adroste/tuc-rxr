#pragma once
#include "../System/Input.h"
#include "OpenGL/Drawing.h"
#include <list>
#include <memory>
#include "../System/Exceptions/ExceptionInvalidOperation.h"
#include "UI/UIObject.h"
#include "UI/UIObjectList.h"
#include "UI/WindowManager.h"

class GameState : public WindowManager, public Input::IReceiver, public Input::IBroadcaster
{
public:
	enum class TransitionState
	{
		Undefined,
		Preserve,
		ForcePreserve,
		Discard,
		// DiscardWithPrevious is stronger than Preserve but weaker than ForcePreserve
		DiscardWithPrevious,
		// Preserve or DiscardWithPrevious on (external) discard (:= superior GameState calls DiscardWithPrevious)
		PreserveOrDiscardWithPrevious
	};

public:
	GameState(bool drawPrevState = false)
		:
		m_drawPrev(drawPrevState)
	{
		Input::registerState(this);
	}
	virtual ~GameState() override
	{
		Input::unregisterState(this);
	}

	virtual bool update(float dt) = 0;

	virtual void composeFrame(Drawing& draw, float dt)
	{
		//draw.rect(RectF(400.0f, 400.0f, 450.0f, 450.0f), Color::Cyan());
		drawUIElements(draw);
	}

	// Input
	virtual bool keyDown(SDL_Scancode s) override
	{
		return sendKeyDown(s);
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		return sendKeyUp(s);
	}
	virtual bool charDown(char c) override
	{
		return sendCharDown(c);
	}
	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		return sendMouseMove(mpos, mdiff, handled);
	}
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		return sendMouseDown(mpos, button);
	}
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		return sendMouseUp(mpos, button);
	}
	virtual bool wheel(const PointF& mpos, float amount) override
	{
		return sendWheel(mpos, amount);
	}


	// Events
	virtual void onResize()
	{
		updateUIElements();
	}


	bool drawPreviousState() const
	{
		return m_drawPrev;
	}

	bool isFinished() const
	{
		assert(!m_isFinished || m_transitionState != TransitionState::Undefined);
		return m_isFinished;
	}

	// nullptr if next state is previous state
	std::unique_ptr<GameState> popNextState()
	{
		m_isFinished = false;
		return std::move(m_pNextState);
	}

	TransitionState getTransitionState() const
	{
		assert(m_transitionState != TransitionState::Undefined);
		return m_transitionState;
	}

protected:
	void setNextState(TransitionState transitionState, std::unique_ptr<GameState> nextState = nullptr)
	{
		assert(!m_pNextState);
		m_pNextState = std::move(nextState);
		m_transitionState = transitionState;
		m_isFinished = true;
	}

private:
	TransitionState m_transitionState = TransitionState::Undefined;
	const bool m_drawPrev;
	std::unique_ptr<GameState> m_pNextState;
	bool m_isFinished = false;
};
