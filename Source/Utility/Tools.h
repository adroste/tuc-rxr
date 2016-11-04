#pragma once
#include <algorithm>
#include <assert.h>
#include <string>

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
		return val2 * scale + val1 * (TScalar(1.0) - scale);
	}

	inline void splitFilenameToPathFile(std::string* dstPath, std::string* dstFile, const std::string& full)
	{
		size_t split = full.find_last_of('/');
		if (split == std::string::npos)
			split = full.find_last_of('\\');

		if (split != std::string::npos)
		{
			if(dstPath) *dstPath = full.substr(0, split + 1);
			if (dstFile) *dstFile = full.substr(split + 1);
		}
		else
		{
			if (dstPath) *dstPath = "";
			if (dstFile) *dstFile = full;
		}
	}
}
