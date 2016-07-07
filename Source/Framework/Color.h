#pragma once

class Color
{
public:
	Color(float r, float g, float b, float a)
		:
		r(r), g(g), b(b), a(a)
	{}
	Color(float r, float g, float b)
		: Color(r, g, b, 1.0f)
	{}
	Color()
		: Color(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	// predefined colors
	static Color Red()
	{
		return Color(1.0f, 0.0f, 0.0f);
	}
	static Color DarkRed()
	{
		return Color(0.5f, 0.0f, 0.0f);
	}
	static Color Green()
	{
		return Color(0.0f, 1.0f, 0.0f);
	}
	static Color DarkGreen()
	{
		return Color(0.0f, 0.5f, 0.0f);
	}
	static Color Blue()
	{
		return Color(0.0f, 0.0f, 1.0f);
	}
	static Color White()
	{
		return Color(1.0f, 1.0f, 1.0f);
	}
	static Color Black()
	{
		return Color(0.0f, 0.0f, 0.0f);
	}
	static Color Yellow()
	{
		return Color(1.0f, 1.0f, 0.0f);
	}
	static Color Cyan()
	{
		return Color(0.0f, 1.0f, 1.0f);
	}
	static Color Magenta()
	{
		return Color(1.0f, 0.0f, 1.0f);
	}
	static Color Gray(float f)
	{
		return Color(f, f, f);
	}
	static Color Gray()
	{
		return Color(0.5f, 0.5f, 0.5f);
	}
	static Color DarkGray()
	{
		return Color(0.2f, 0.2f, 0.2f);
	}

public:
	float r, g, b, a;
};