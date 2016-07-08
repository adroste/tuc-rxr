#pragma once
#include "../SDL/SDL.h"
#include "../SDL/SDL_timer.h"
#include <assert.h>

class Timer
{
public:
	using Timestamp = Uint64;

	void startWatch()
	{
		m_running = true;
		m_start = SDL_GetPerformanceCounter();
	}
	void stopWatch()
	{
		assert(m_running);
		if (m_running)
		{
			m_current = SDL_GetPerformanceCounter();
			m_running = false;
		}
	}
	float getTimeMilli() const
	{
		auto cur = m_current;
		if (m_running)
		{
			cur = SDL_GetPerformanceCounter();
		}

		return float(cur - m_start) * getInvFreqMilli();
	}
	float getTimeSecond() const
	{
		auto cur = m_current;
		if (m_running)
		{
			cur = SDL_GetPerformanceCounter();
		}

		return float(cur - m_start) * getInvFreq();
	}
	float lapSecond()
	{
		assert(m_running);
		m_current = getTimestamp();
		float time = float(m_current - m_start) * getInvFreq();
		m_start = m_current;
		return time;
	}
	static Timestamp getTimestamp()
	{
		return SDL_GetPerformanceCounter();
	}
	static float convertMilli(const Timestamp& val1, const Timestamp& val2)
	{
		return float(val2 - val1) * getInvFreqMilli();
	}
	static float convertMilli(const Timestamp& start)
	{
		return convertMilli(start, getTimestamp());
	}
private:
	static double getInvFreqDouble()
	{
		static double invFreq = 1.0 / double(SDL_GetPerformanceFrequency());
		return invFreq;
	}
	static float getInvFreq()
	{
		static float freq = float(getInvFreqDouble());
		return freq;
	}
	static float getInvFreqMilli()
	{
		static float freq = float(getInvFreqDouble() * 1000.0);
		return freq;
	}
private:
	Timestamp m_start = 0;
	Timestamp m_current = 0;
	bool m_running = false;
};