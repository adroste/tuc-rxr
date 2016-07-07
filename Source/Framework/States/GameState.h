#pragma once
#include "../../System/Input.h"
#include <list>

class GameState : public Input::IReceiver
{
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
	virtual void composeFrame(float dt) = 0;
	bool drawPreviousState() const
	{
		return m_drawPrev;
	}

	virtual bool keyDown(SDL_Scancode s) override
	{
		int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->keyDown(s))
				quit = true;
		}
		return quit;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->keyUp(s))
				quit = true;
		}
		return quit;
	}
	virtual bool charDown(char c) override
	{
		int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->charDown(c))
				quit = true;
		}
		return quit;
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		// TODO ? handled ???
		for (auto r : m_receiver)
		{
			if (r->mouseMove(mpos, handled))
				handled = true;
		}
		return handled;
	}

	virtual bool mouseDown(Uint8 button, const PointF& mpos) override // TODO enum
	{
		int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->mouseDown(button, mpos))
				quit = true;
		}
		return quit;
	}
	virtual bool mouseUp(Uint8 button, const PointF& mpos) override
	{
		int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->mouseUp(button, mpos))
				quit = true;
		}
		return quit;
	}
	virtual bool wheel(float amount) override
	{
		int curZ = -1;
		bool quit = false;
		for (auto r : m_receiver)
		{
			if (quit && curZ != r->getZIndex())
				break;
			curZ = r->getZIndex();
			if (r->wheel(amount))
				quit = true;
		}
		return quit;
	}

private:
	const bool m_drawPrev;
	std::list<Input::IReceiver*> m_receiver;
};
