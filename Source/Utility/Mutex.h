#pragma once
#include "../SDL/SDL_mutex.h"
#include "../System/Exceptions/SDL_Exception.h"

class Mutex final
{
	friend class LockGuard;
	friend class Condition;
public:
	Mutex()
	{
		m_pMutex = SDL_CreateMutex();
		if (!m_pMutex)
			throw SDL_Exception("Mutex::Mutex mutex creation failed");
	}
	~Mutex()
	{
		SDL_DestroyMutex(m_pMutex);
		m_pMutex = nullptr;
	}
	Mutex(const Mutex&) = delete;
	Mutex& operator=(const Mutex&) = delete;	

private:
	void lock() const
	{
		if (SDL_LockMutex(m_pMutex) != 0)
			throw SDL_Exception("Mutex::lock failed");
	}
	void unlock() const
	{
		if (SDL_UnlockMutex(m_pMutex) != 0)
			throw SDL_Exception("Mutex::unlock failed");
	}

private:
	SDL_mutex* m_pMutex;
};

class LockGuard final
{
	friend class Condition;
public:
	LockGuard(const Mutex& mu)
		:
		m_mutex(mu)
	{
		m_mutex.lock();
	}
	LockGuard(const LockGuard&) = delete;
	LockGuard& operator=(const LockGuard&) = delete;
	void unlock()
	{
		if (m_locked)
		{
			m_mutex.unlock();
			m_locked = false;
		}
	}
	~LockGuard()
	{
		unlock();
	}

private:
	const Mutex& m_mutex;
	bool m_locked = true;
};