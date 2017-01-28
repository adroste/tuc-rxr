#pragma once
#include "../Utility/Tools.h"
#include <glm/glm.hpp>

class Color
{
public:
	constexpr Color(float r, float g, float b, float a)
		:
		r(r), g(g), b(b), a(a)
	{}
	constexpr Color(float r, float g, float b)
		: Color(r, g, b, 1.0f)
	{}

	explicit constexpr Color(uint32_t dword)
		:
		r(((dword & 0xFF0000) >> 16) / 255.0f),
		g(((dword & 0xFF00) >> 8) / 255.0f),
		b((dword & 0xFF) / 255.0f),
		a(((dword) >> 24) / 255.0f)
	{}
	uint32_t toDWORD()
	{
		normalize();
		uint32_t r = uint8_t(this->b * 255.0f);
		r |= uint8_t(this->g * 255.0f) << 8;
		r |= uint8_t(this->r * 255.0f) << 16;
		r |= uint8_t(this->a * 255.0f) << 24;
		return r;
	}
	glm::vec3 toVec3() const
	{
		return glm::vec3(r, g, b);
	}

	Color()
		: Color(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	// operators
	Color& operator+=(const Color& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		normalize();
		return *this;
	}
	Color& operator+(const Color& rhs) const
	{
		Color c(*this);
		return c += rhs;
	}
	Color& operator*=(const Color& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		a *= rhs.a;
		normalize();
		return *this;
	}
	Color& operator*(const Color& rhs) const
	{
		Color c(*this);
		return c *= rhs;
	}
	Color& operator*=(float scalar)
	{
		r *= scalar;
		g *= scalar;
		b *= scalar;
		//a *= scalar;
		normalize();
		return *this;
	}
	Color operator*(float scalar) const
	{
		Color c(*this);
		return c *= scalar;
	}

	// additional functionality

	// red + green + blue
	float getColorSum() const
	{
		return r + g + b;
	}

	// indicates how bright the color is for the human eye
	float getLuminance() const
	{
		return (*this * getLuminanceConst()).getColorSum();
	}

	Color toGamma() const
	{
		return Color(std::pow(r, 2.2f), std::pow(g, 2.2f), std::pow(b, 2.2f), a);
	}
	Color fromGamma() const
	{
		return Color(std::pow(r, 1.0f / 2.2f), std::pow(g, 1.0f / 2.2f), std::pow(b, 1.0f / 2.2f), a);
	}

	static Color mix(Color c1, Color c2, float fac)
	{
		return tool::mix(c1, c2, fac);
	}

	Color& normalize()
	{
		r = tool::clamp(r, 0.0f, 1.0f);
		g = tool::clamp(g, 0.0f, 1.0f);
		b = tool::clamp(b, 0.0f, 1.0f);
		a = tool::clamp(a, 0.0f, 1.0f);

		return *this;
	}

	// predefined colors
	static constexpr Color Red()
	{
		return Color(1.0f, 0.0f, 0.0f);
	}
	static constexpr Color DarkRed()
	{
		return Color(0.5f, 0.0f, 0.0f);
	}
	static constexpr Color Green()
	{
		return Color(0.0f, 1.0f, 0.0f);
	}
	static constexpr Color DarkGreen()
	{
		return Color(0.0f, 0.5f, 0.0f);
	}
	static constexpr Color Blue()
	{
		return Color(0.0f, 0.0f, 1.0f);
	}
	static constexpr Color White()
	{
		return Color(1.0f, 1.0f, 1.0f);
	}
	static constexpr Color Black()
	{
		return Color(0.0f, 0.0f, 0.0f);
	}
	static constexpr Color Yellow()
	{
		return Color(1.0f, 1.0f, 0.0f);
	}
	static constexpr Color Cyan()
	{
		return Color(0.0f, 1.0f, 1.0f);
	}
	static constexpr Color Magenta()
	{
		return Color(1.0f, 0.0f, 1.0f);
	}
	static constexpr Color Gray(float f)
	{
		return Color(f, f, f);
	}
	static constexpr Color Gray()
	{
		return Color(0.5f, 0.5f, 0.5f);
	}
	static constexpr Color DarkGray()
	{
		return Color(0.2f, 0.2f, 0.2f);
	}
	static constexpr Color Brown()
	{
		return Color(0.72f, 0.47f, 0.34f);
	}
	static Color Random()
	{
		float rx = (rand() % 256) / 256.0f;
		float ry = (rand() % 256) / 256.0f;
		float rz = (rand() % 256) / 256.0f;
		return Color(rx, ry, rz);
	}

	static constexpr Color getLuminanceConst()
	{
		return Color(0.299f, 0.587f, 0.114f, 0.0f);
	}
public:
	float r, g, b, a;
};
