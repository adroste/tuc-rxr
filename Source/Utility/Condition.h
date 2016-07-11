#pragma once
#include <functional>
#include <assert.h>
#include "../SDL/SDL.h"
#include "../SDL/SDL_mutex.h"
#include "SDL_Exception.h"
#include "../System/Log.h"
#include "Mutex.h"

class Cond final
{
public:
	Cond()
	{
		m_pCond = SDL_CreateCond();
		if (!m_pCond)
			throw new SDL_Exception("Cond::Cond condition creation failed");
	}
	~Cond()
	{
		SDL_DestroyCond(m_pCond);
		m_pCond = nullptr;
	}
	Cond(const Cond&) = delete;
	Cond& operator=(const Cond&) = delete;

	void wait(const Mutex& mutex, int timeoutms = -1) const
	{
		++m_waiting;
		if (timeoutms <= 0)
		{
			if (SDL_CondWait(m_pCond, mutex.m_pMutex) != 0)
				throw SDL_Exception("Cond::wait condition wait failed");
		}
		else
		{
			auto r = SDL_CondWaitTimeout(m_pCond, mutex.m_pMutex, Uint32(timeoutms));
			if (r == SDL_MUTEX_TIMEDOUT)
				Log::warning("Cond::wait timeout (" + std::to_string(timeoutms) + "ms) waiting for condition");
			else if (r < 0)
				throw SDL_Exception("Cond::wait condition wait (+timeout) failed");

		}
		--m_waiting;
	}

	void wait(const Mutex& mutex, std::function<bool(void)> pred, int timeoutms = -1) const
	{
		while (!pred())
		{
			wait(mutex, timeoutms);
		}
	}

	void signal() const
	{
		assert(m_pCond);
		if (SDL_CondSignal(m_pCond) != 0)
			throw SDL_Exception("Cond::signal condition signal failed");
	}

	void broadcast() const
	{
		assert(m_pCond);
		if (SDL_CondBroadcast(m_pCond) != 0)
			throw SDL_Exception("Cond::broadcast condition broadcast failed");
	}

	int getWaiting() const
	{
		return m_waiting;
	}

private:
	SDL_cond* m_pCond;
	mutable int m_waiting = 0;
};