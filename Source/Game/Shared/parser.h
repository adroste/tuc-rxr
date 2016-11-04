#pragma once
#include <string>
#include "../../Framework/Color.h"

// parser utilities
namespace parser
{
	inline std::string colToString(uint32_t c)
	{
		char buffer[16];
		sprintf(buffer, "0x%08x", c);
		return std::string(buffer);
	}

	inline std::string colToString(const glm::vec3& v)
	{
		Color c = Color(v.r, v.g, v.b);
		return colToString(c.toDWORD());
	}

	inline uint32_t strToColor(const char* s)
	{
		auto str = std::string(s);
		if (str.size() > 2)
		{
			str = str.substr(2, str.length() - 2);
			try
			{
				auto i = uint32_t(int32_t(std::strtoll(s, nullptr, 16)));
				return i;
			}
			catch (const std::exception&) {}
		}
		return 0;
	}

	inline float getFloat(const char* s)
	{
		return float(atof(s));
	}

	inline int getInt(const char* s)
	{
		return atoi(s);
	}

	inline std::string vecToString(const glm::vec3& v)
	{
		return std::to_string(v.r) + " " + std::to_string(v.g) + " " + std::to_string(v.b);
	}

	inline glm::vec3 strToVec3(const char* s)
	{
		glm::vec3 v;
		sscanf(s, "%f %f %f", &v.r, &v.g, &v.b);
		return v;
	}
}