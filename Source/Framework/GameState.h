#pragma once
#include "../System/Input.h"
#include "OpenGL/Drawing.h"
#include <list>
#include <memory>
#include "../System/Exceptions/ExceptionInvalidOperation.h"

class GameState : public Input::IReceiver
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
	virtual ~GameState()
	{
		Input::unregisterState(this);
	}

	void sortReceivers()
	{
		// TODO performance? #jan
		m_receivers.sort([](Input::IReceiver* lr, Input::IReceiver* rr)
		{
			return lr->getZIndex() > rr->getZIndex();
		});
	}

	void regReceiver(Input::IReceiver* rec)
	{
		for (auto r : m_receivers)
			if (r == rec)
				throw ExceptionInvalidOperation("GameState::regReceiver try to add receiver twice", "receiver already in m_receivers");
		m_receivers.push_front(rec);
		sortReceivers();
	}

	void unregReceiver(Input::IReceiver* rec)
	{
		m_receivers.remove_if([rec](Input::IReceiver* curRec)
		{
			return rec == curRec;
		});
	}

	virtual bool update(float dt) = 0;
	virtual void composeFrame(Drawing& draw, float dt) = 0;
	bool drawPreviousState() const
	{
		return m_drawPrev;
	}

	virtual bool keyDown(SDL_Scancode s) override
	{
		return handleKey(&Input::IReceiver::keyDown, s);
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		return handleKey(&Input::IReceiver::keyUp, s);
	}
	virtual bool charDown(char c) override
	{
		return handleKey(&Input::IReceiver::charDown, c);
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		int curZ = -1;
		bool prevHandled = handled;

		for (auto r : m_receivers)
		{
			if(curZ != r->getZIndex())
			{
				prevHandled = handled;
			}

			bool nowHandled = r->mouseMove(mpos, prevHandled);
			handled = nowHandled || handled;
		}
		return handled;
	}
	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		return handleKey(&Input::IReceiver::mouseDown, button, mpos);
	}
	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		return handleKey(&Input::IReceiver::mouseUp, button, mpos);
	}
	virtual bool wheel(float amount, const PointF& mpos) override
	{
		return handleKey(&Input::IReceiver::wheel, amount, mpos);
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

private:
	template <typename memFunc, typename... ArgT>
	bool handleKey(memFunc pFunc, ArgT... args)
	{
		int curZ = -1;
		bool handled = false;
		for (auto r : m_receivers)
		{
			if (handled && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (((*r).*pFunc)(args...))
				handled = true;
		}
		return handled;
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
	std::list<Input::IReceiver*> m_receivers;
	std::unique_ptr<GameState> m_pNextState;
	bool m_isFinished = false;
};
