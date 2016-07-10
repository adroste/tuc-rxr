#pragma once
#include <algorithm>
#include <assert.h>

namespace tool
{
	template <class T>
	T clamp(const T& val,const T& min,const T& max)
	{
		return std::min(max, std::max(min, val));
	}
	
	template <class T, class TScalar>
	T mix(const T& val1, const T& val2, const TScalar& scale)
	{
		assert(scale >= TScalar(0.0));
		assert(scale <= TScalar(1.0));
		return val1 * scale + val2 * (TScalar(1.0) - scale);
	}
}