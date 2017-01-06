#pragma once
#include <cmath>
#include <assert.h>
#include <algorithm>

template <class T>
class Point
{
public:
	T x, y;
	Point(T x, T y)
		: x(x), y(y)
	{}
	Point()
		:
		Point(T(0.0), T(0.0))
	{}
	explicit Point(T xy)
		:
		x(xy), y(xy)
	{}
	template <class T2, class T3>
	Point(T2 x, T3 y)
		:
		x(T(x)),
		y(T(y))
	{}
	template <class T2>
	Point(const Point<T2>& copy)
	{
		x = T(copy.x);
		y = T(copy.y);
	}
	//operators
	inline bool operator==(const Point<T>& rhs) const
	{
		if (x == rhs.x)
			if (y == rhs.y)
				return true;
		return false;
	}
	inline bool operator!=(const Point<T>& rhs) const
	{
		return !(*this == rhs);
	}
	inline bool isEqual(const Point<T>& other, T tolerance)
	{
		if (x >= other.x - tolerance)
			if (x <= other.x + tolerance)
				if (y >= other.y - tolerance)
					if (y <= other.y + tolerance)
						return true;
		return false;
	}
	template <class T2>
	inline Point<T>& operator=(const Point<T2>& rhs)
	{
		x = T(rhs.x);
		y = T(rhs.y);
		return *this;
	}
	inline Point<T>& operator=(const Point<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	inline Point<T>& operator+=(const Point<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	inline Point<T>& operator-=(const Point<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	inline Point<T> operator+(const Point<T>& rhs) const
	{
		Point<T> copy = *this;
		return copy += rhs;
	}
	inline Point<T> operator-(const Point<T>& rhs) const
	{
		Point<T> copy = *this;
		return copy -= rhs;
	}
	inline Point<T> operator-() const
	{
		return Point<T>(*this) * (T)-1.0;
	}
	//vector
	inline Point<T>& operator*=(const T& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	inline Point<T> operator*(const T& rhs) const
	{
		Point<T> res = *this;
		return (res *= rhs);
	}
	inline Point<T>& operator/=(const T& rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}
	inline Point<T> operator/(const T& rhs) const
	{
		Point<T> res = *this;
		return (res /= rhs);
	}
	inline T operator*(const Point<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}

	//operators concerning the length
	inline bool operator>(const Point<T>& rhs)
	{
		return lengthSq() > rhs.lengthSq();
	}
	inline bool operator<(const Point<T>& rhs)
	{
		return lengthSq() < rhs.lengthSq();
	}
	inline bool operator>=(const Point<T>& rhs)
	{
		return lengthSq() >= rhs.lengthSq();
	}
	inline bool operator<=(const Point<T>& rhs)
	{
		return lengthSq() <= rhs.lengthSq();
	}

	//helper functions
	inline T lengthSq() const
	{
		return x * x + y * y;
	}
	inline T length() const
	{
		return std::sqrt(lengthSq());
	}
	inline Point<T> normalize() const
	{
		Point<T> res(*this);
		T len = length();
		//assert(len != (T)0);
		if (len != static_cast<T>(0))
			return res /= len;
		else
			return res;
	}
	inline bool isClose(const Point<T>& pt, const float maxDistance) const
	{
		return ((maxDistance * maxDistance) > (((*this) - pt).lengthSq()));
	}
	inline T det(const Point<T>& rhs) const
	{
		return x * rhs.y - y * rhs.x;
	}
	inline Point<T> zeroX() const
	{
		return Point < T > {T(0.0), y};
	}
	inline Point<T> zeroY() const
	{
		return Point < T > {x, T(0.0)};
	}
	inline bool isParallel(const Point<T>& other) const
	{
		// check if not lu
		T n;
		if (x != 0)
		{
			n = other.x / x;
		}
		else if (y != 0)
		{
			n = other.y / y;
		}
		else
		{
			n = T(1.0);
		}
		return ((*this) * n == other);
	}

	Point<T> fromIndex(T cindex) const
	{
		Point<T> p;
		p.y = cindex / x;
		p.x = cindex % x;
		return p;
	}
	T calcIndex(const Point<T>& p) const
	{
		assert(p.x < x);
		assert(p.y < y);
		return x * p.y + p.x;
	}
	T size() const
	{
		return x * y;
	}

	enum class Side
	{
		Left,
		Right
	};
	inline Side whichSideVec(const Point<T>& vec)
	{
		if (normalize().det(vec.normalize()) > T(0.0))
		{
			return Side::Left;
		}
		else
		{
			return Side::Right;
		}
	}
	//Rotation
	inline Point<T> cw90() const
	{
		return Point < T > {y, -x};
	}
	inline Point<T> ccw90() const
	{
		return Point< T > {-y, x};
	}
	inline Point<T> cw45() const
	{
		return (*this + (*this).cw90()) / 2;
	}
	inline Point<T> ccw45() const
	{
		return (*this + (*this).cw90()) / 2;
	}
	inline Point<T> rotate(float angle) const
	{
		Point<T> res = *this;
		float cost = cos(angle);
		float sint = sin(angle);
		// math magic
		res.x = x * cost - y * sint;
		res.y = y * cost + x * sint;
		return res;
	}
	static inline Point<T> mult(const Point<T>& a, const Point<T>& b)
	{
		return { a.x * b.x, a.y * b.y };
	}
};

using PointF = Point< float >;
using PointI = Point< int >;
using PointS = Point< size_t >;