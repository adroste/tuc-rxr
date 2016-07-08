#pragma once
#include "Point.h"
#include <algorithm>

template <class T>
class Rect
{
public:
	union
	{
		struct
		{
			T
				x1,
				y1,
				x2,
				y2;
		};
		struct
		{
			T
				left,
				top,
				right,
				bottom;
		};

		struct
		{
			Point<T> p1; //top left
			Point<T> p2; //bottom right
		};
	};
	Rect()
	{
		Rect(T(0.0), T(0.0), T(0.0), T(0.0));
	}
	Rect(T left, T top, T right, T bottom)
		:
		left(left), right(right), top(top), bottom(bottom)
	{}
	Rect(const Point<T>& po1, const Point<T>& po2)
		:
		x1(std::min(po1.x, po2.x)),
		x2(std::max(po1.x, po2.x)),
		y1(std::min(po1.y, po2.y)),
		y2(std::max(po1.y, po2.y))
	{}
	template <class T2>
	Rect(const Rect<T2>& copy)
		:
		Rect(T(copy.x1), T(copy.y1), T(copy.x2), T(copy.y2))
	{

	}
	//operator
	inline Rect<T>& operator=(const Rect<T>& rhs)
	{
		x1 = rhs.x1;
		x2 = rhs.x2;
		y1 = rhs.y1;
		y2 = rhs.y2;
		return *this;
	}
	inline Rect<T>& operator+=(const Point<T>& pt)
	{
		p1 += pt;
		p2 += pt;
		return *this;
	}
	inline Rect<T> operator+(const Point<T>& pt) const
	{
		Rect<T> t = *this;
		t += pt;
		return t;
	}
	inline Rect<T>& operator-=(const Point<T>& pt)
	{
		p1 -= pt;
		p2 -= pt;
		return *this;
	}
	inline Rect<T> operator-(const Point<T>& pt) const
	{
		Rect<T> t = *this;
		t -= pt;
		return t;
	}

	//other
	inline T getWidth() const
	{
		return (x2 - x1);
	}
	inline T getHeight() const
	{
		return (y2 - y1);
	}

	//functions
	inline bool isPointInside(const Point<T>& pt) const
	{
		if (pt.x >= left)
			if (pt.x <= right)
				if (pt.y >= top)
					if (pt.y <= bottom)
						return true;
		return false;
	}
	inline bool isRectInside(const Rect<T>& re) const
	{
		if (PointInside(re.getTopLeft()))
			if (PointInside(re.getBottomRight()))
				if (PointInside(re.getTopRight()))
					if (PointInside(re.getBottomLeft()))
						return true;
		return false;
	}
	inline bool isRectCutting(const  Rect<T>& re) const
	{
		if (PointInside(re.getTopLeft()) || PointInside(re.getTopRight()) ||
			PointInside(re.getTopRight()) || PointInside(re.getBottomLeft()))
			return true;
		return false;
	}
	//clip this rect into another
	inline Rect<T> clipTo(const Rect<T>& o) const
	{
		assert(isRectCutting(o));

		Rect<T> r;
		r.x1 = std::max(o.x1, x1);
		r.x2 = std::min(o.x2, x2);

		r.y1 = std::max(o.y1, y1);
		r.y2 = std::min(o.y2, y2);

		r.x1 = std::min(r.x1, r.x2);
		r.y1 = std::min(r.y1, r.y2);

		r.x2 = std::min(r.x2, x2);
		r.y2 = std::min(r.y2, y2);

		return r;
	}
	inline Point<T> getDim() const
	{
		return Point<T>(getWidth(), getHeight());
	}
	inline Point<T> getTopRight() const
	{
		return Point<T>(x2, y1);
	}
	inline Point<T> getTopLeft() const
	{
		return p1;
	}
	inline Point<T> getBottomRight() const
	{
		return p2;
	}
	inline Point<T> getBottomLeft() const
	{
		return Point<T>(x1, y2);
	}
	inline Point<T> getMidpoint() const
	{
		return (p1 + p2) / T(2.0);
	}
	static Rect<T> constructFromPoint(const Point<T>& p, T radius)
	{
		return Rect<T>(p.x - radius, p.y - radius, p.x + radius, p.y + radius);
	}
	inline Rect<T> shrink(T fac) const
	{
		Rect<T> res = *this;
		res.x1 += fac;
		res.y1 += fac;
		res.x2 -= fac;
		res.y2 -= fac;
		return res;
	}
};

using RectF = Rect<float>;
using RectI = Rect<int>;