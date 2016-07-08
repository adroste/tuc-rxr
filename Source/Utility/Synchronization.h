#pragma once
#include <functional>
#include "../SDL/SDL.h"
#include "../SDL/SDL_mutex.h"
#include "SDL_Exception.h"
#include "../System/Log.h"

class SyncContext final
{
	friend class Syncer;
public:
	SyncContext()
	{}
	~SyncContext()
	{
		destroyCond();
		destroyMutex();
	}
	SyncContext(const SyncContext&) = delete;
	SyncContext& operator=(const SyncContext&) = delete;

private:
	void createMutex()
	{
		if (!m_pMutex)
		{
			m_pMutex = SDL_CreateMutex();
			if (!m_pMutex)
				throw SDL_Exception("SyncContext::createMutex mutex creation failed");
		}
	}

	void destroyMutex()
	{
		if (m_pMutex)
		{
			SDL_DestroyMutex(m_pMutex);
			m_pMutex = nullptr;
		}
	}

	void createCond()
	{
		if (!m_pCond)
		{
			m_pCond = SDL_CreateCond();
			if (!m_pCond)
				throw SDL_Exception("SyncContext::createCond condition creation failed");
		}
	}

	void destroyCond()
	{
		if (m_pCond)
		{
			SDL_DestroyCond(m_pCond);
			m_pCond = nullptr;
		}
	}

private:
	SDL_mutex* m_pMutex = nullptr;
	int m_locked = 0;
	SDL_cond* m_pCond = nullptr;
	int m_waiting = 0;
};

class Syncer final
{
public:
	Syncer(SyncContext& syncContext, bool autolock = true)
		:
		m_syncContext(syncContext),
		m_autolock(autolock)
	{
		if (m_autolock)
			lock();
	}
	~Syncer()
	{
		unlock();
	}
	Syncer(const Syncer&) = delete;
	Syncer& operator=(const Syncer&) = delete;

	void lock() const
	{
		m_syncContext.createMutex();

		if (SDL_LockMutex(m_syncContext.m_pMutex) != 0)
			throw SDL_Exception("Syncer::lock mutex lock failed");
		++m_syncContext.m_locked;
	}

	void unlock() const
	{
		if (!m_syncContext.m_pMutex) return;

		if (m_syncContext.m_locked > 0)
		{
			if (SDL_UnlockMutex(m_syncContext.m_pMutex) != 0)
				throw SDL_Exception("Syncer::unlock mutex unlock failed");
			--m_syncContext.m_locked;
		}
	}

	void wait(std::function<bool(void)> pred, int timeoutms = -1) const
	{
		m_syncContext.createMutex();
		m_syncContext.createCond();

		while (!pred())
		{
			++m_syncContext.m_waiting;
			if (timeoutms <= 0)
			{
				if (SDL_CondWait(m_syncContext.m_pCond, m_syncContext.m_pMutex) != 0)
					throw SDL_Exception("Syncer::wait condition wait failed");
			}
			else
			{
				int r = SDL_CondWaitTimeout(m_syncContext.m_pCond, m_syncContext.m_pMutex, Uint32(timeoutms));
				if (r == SDL_MUTEX_TIMEDOUT)
					Log::warning("Syncer::wait timeout (" + std::to_string(timeoutms) + "ms) waiting for condition");
				else if (r < 0)
					throw SDL_Exception("Syncer::wait condition wait (+timeout) failed");
				
			}
			--m_syncContext.m_waiting;
		}
	}

	void signal(bool unlockImmediately = false) const
	{
		// if no one is waiting, no one has to be signaled
		if (m_syncContext.m_pCond && m_syncContext.m_waiting > 0)
		{			
			if (SDL_CondSignal(m_syncContext.m_pCond) != 0)
				throw SDL_Exception("Syncer::signal condition signal failed");
		}
		if (unlockImmediately)
			unlock();
	}

private:
	SyncContext& m_syncContext;
	bool m_autolock;
};