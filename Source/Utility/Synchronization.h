#pragma once
#include "../SDL/SDL.h"
#include "../SDL/SDL_mutex.h"
#include "SDL_Exception.h"
#include "../System/Log.h"

class SyncContext final
{
	friend class Syncer;
public:
	bool condition = false;

public:
	SyncContext() {}
	~SyncContext() {}
	SyncContext(const SyncContext&) = delete;
	SyncContext& operator=(const SyncContext&) = delete;

private:
	SDL_mutex* m_pMutex = nullptr;
	bool m_locked = false;
	SDL_cond* m_pCond = nullptr;
	int m_waiting = 0;
};

class Syncer final
{
public:
	Syncer(SyncContext& syncContext, bool autolock = true)
		:
		m_syncContext(syncContext)
	{
		if (autolock)
			lock();
	}
	~Syncer()
	{
		unlock();
		destroyCond();
		destroyMutex();
	}
	Syncer(const Syncer&) = delete;
	Syncer& operator=(const Syncer&) = delete;

	void lock()
	{
		createMutex();

		if (SDL_LockMutex(m_syncContext.m_pMutex) != 0)
			throw SDL_Exception("Syncer::lock mutex lock failed");
		m_syncContext.m_locked = true;
	}

	void unlock()
	{
		if (!m_syncContext.m_pMutex) return;

		if (m_syncContext.m_locked)
		{
			if (SDL_UnlockMutex(m_syncContext.m_pMutex) != 0)
				throw SDL_Exception("Syncer::unlock mutex unlock failed");
			m_syncContext.m_locked = false;
		}
	}

	void wait(int timeoutms = -1)
	{
		createMutex();
		createCond();

		while (!m_syncContext.condition)
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
			m_syncContext.condition = true;
		}
	}

	void signal(bool autounlock = true)
	{
		// if no one is waiting, no one has to be signaled
		if (m_syncContext.m_pCond && m_syncContext.m_waiting > 0)
		{			
			if (SDL_CondSignal(m_syncContext.m_pCond) != 0)
				throw SDL_Exception("Syncer::signal condition signal failed");
		}
		if (autounlock)
			unlock();
	}

private:
	void createMutex()
	{
		if (!m_syncContext.m_pMutex)
		{
			m_syncContext.m_pMutex = SDL_CreateMutex();
			if (!m_syncContext.m_pMutex)
				throw SDL_Exception("Syncer::createMutex mutex creation failed");
		}
	}

	void destroyMutex()
	{
		if (m_syncContext.m_pMutex)
			SDL_DestroyMutex(m_syncContext.m_pMutex);
	}

	void createCond()
	{
		if (!m_syncContext.m_pCond)
		{
			m_syncContext.m_pCond = SDL_CreateCond();
			if (!m_syncContext.m_pCond)
				throw SDL_Exception("Syncer::createCond condition creation failed");
		}
	}

	void destroyCond()
	{
		if (m_syncContext.m_pCond)
			SDL_DestroyCond(m_syncContext.m_pCond);
	}

private:
	SyncContext& m_syncContext;
};