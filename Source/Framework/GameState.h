#pragma once
#include "../System/Input.h"
#include "OpenGL/Drawing.h"
#include <list>
#include <memory>
#include "../System/Exceptions/ExceptionInvalidOperation.h"
#include "UI/UIObject.h"
#include "UI/UIObjectList.h"
#include "UI/WindowManager.h"

class GameState : public WindowManager
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
	{}
	virtual ~GameState() override
	{}

	virtual bool update(float dt) = 0;

	virtual void composeFrame(Drawing& draw, float dt)
	{
		drawLayer(draw);
	}

	// Events
	virtual void onResize()
	{
		updateLayer();
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
