#pragma once
#include "../../System/Input.h"
#include "../OpenGL/Drawing.h"
#include <list>
#include <memory>

class GameState : public Input::IReceiver
{
public:
	enum class TransitionState
	{
		Preserve,
		ForcePreserve,
		Discard,
		// DiscardWithPrevious is stronger than Preserve but weaker than ForcePreserve
		DiscardWithPrevious
	};

public:
	GameState(TransitionState transitionState, bool drawPrevState = false)
		:
		m_transitionState(transitionState),
		m_drawPrev(drawPrevState)
	{
		Input::registerState(this);
	}
	virtual ~GameState()
	{
		Input::unregisterState(this);
	}

	void regReceiver(Input::IReceiver* rec)
	{
		m_receiver.push_back(rec);
		// TODO performance? / right order?
		m_receiver.sort([](Input::IReceiver* lr, Input::IReceiver* rr)
		{
			return lr->getZIndex() > rr->getZIndex();
		});
	}

	void unregReceiver(Input::IReceiver* rec)
	{
		m_receiver.remove_if([rec](Input::IReceiver* curRec)
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
	/*	int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->keyDown(s))
				quit = true;
		}
		return quit;*/
		return handleKey(&Input::IReceiver::keyDown, s);
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		/*int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->keyUp(s))
				quit = true;
		}
		return quit;*/
		return handleKey(&Input::IReceiver::keyUp, s);
	}
	virtual bool charDown(char c) override
	{
		/*int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->charDown(c))
				quit = true;
		}
		return quit;*/
		return handleKey(&Input::IReceiver::charDown, c);
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		int curZ = -1;
		bool prevHandled = handled;

		for (auto r : m_receiver)
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
		/*int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->mouseDown(button, mpos))
				quit = true;
		}
		return quit;*/
		return handleKey(&Input::IReceiver::mouseDown, button, mpos);
	}
	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		/*int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->mouseUp(button, mpos))
				quit = true;
		}
		return quit;*/
		return handleKey(&Input::IReceiver::mouseUp, button, mpos);
	}
	virtual bool wheel(float amount) override
	{
		/*int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->wheel(amount))
				quit = true;
		}
		return quit;*/
		return handleKey(&Input::IReceiver::wheel, amount);
	}

	bool isFinished() const
	{
		return m_pNextState.get() != nullptr;
	}

	// nullptr if next state is previous state
	std::unique_ptr<GameState> popNextState()
	{
		return std::move(m_pNextState);
	}

	TransitionState getTransitionState() const
	{
		return m_transitionState;
	}

private:
	template <typename memFunc, typename... ArgT>
	bool handleKey(memFunc pFunc, ArgT... args)
	{
		int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (((*r).*pFunc)(args...))
				quit = true;
		}
		return quit;
	}

protected:
	void setNextState(std::unique_ptr<GameState> nextState)
	{
		assert(!m_pNextState);
		m_pNextState = std::move(nextState);
	}

private:
	TransitionState m_transitionState;
	const bool m_drawPrev;
	std::list<Input::IReceiver*> m_receiver;
	std::unique_ptr<GameState> m_pNextState;
};
