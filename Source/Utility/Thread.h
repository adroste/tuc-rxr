#pragma once
#include "../SDL/SDL.h"
#include "../SDL/SDL_thread.h"
#include <exception>
#include "SDL_Exception.h"
#include <assert.h>
#include "../System/System.h"
#include "Exception.h"

class Thread
{
protected:
	enum Priority
	{
		LOW = SDL_THREAD_PRIORITY_LOW,
		NORMAL = SDL_THREAD_PRIORITY_NORMAL,
		HIGH = SDL_THREAD_PRIORITY_HIGH
	};

public:
	Thread()
	{}
	Thread(const Thread&) = delete;
	Thread& operator=(const Thread&) = delete;
	virtual ~Thread()
	{
		assert(m_pThread == nullptr);
		// TODO maybe log
	}

	void begin()
	{
		assert(!m_isRunning);
		if (!m_isRunning)
		{
			m_pThread = SDL_CreateThread(callback, "x", static_cast<void*>(this));
			if (m_pThread == nullptr)
				throw SDL_Exception("Thread::begin thread creation failed");
			m_isRunning = true;
		}
		else throw Exception("Thread::begin thread already running");
	}
	void detach()
	{
		if (m_pThread)
		{
			SDL_DetachThread(m_pThread);
			m_pThread = nullptr;
		}
	}
	void join()
	{
		if (m_pThread)
		{
			SDL_WaitThread(m_pThread, &m_retVal);
			m_pThread = nullptr;
		}
	}

	bool hasThreadError() const
	{
		return m_threadError.length() != 0;
	}
	std::string getThreadError()
	{
		return m_threadError;
	}

	bool isRunning() const
	{
		return m_isRunning;
	}
protected:
	virtual int threadProc() = 0;

	void setThreadError(const std::string& m)
	{
		m_threadError = m;
	}

	void setThreadPriority(Priority p)
	{
		SDL_SetThreadPriority(SDL_ThreadPriority(p));
	}

private:
	static int callback(void* arg)
	{
		int ret = -1;
		Thread* pThread = static_cast<Thread*>(arg);
		
		// TODO Log errors
		try
		{
			ret = pThread->threadProc();
		}
		catch (const std::exception& e)
		{
			pThread->setThreadError(e.what());
		}
		catch (...)
		{
			pThread->setThreadError("UNKNOWN THREAD ERROR");
		}
		assert(ret == 0);

		return ret;
	}

private:
	SDL_Thread* m_pThread = nullptr;
	int m_retVal;
	std::string m_threadError;
	bool m_isRunning = false;
};