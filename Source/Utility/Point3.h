#pragma once
#include <cmath>
#include <assert.h>
#include <algorithm>
#include <glm/glm.hpp>

template <class T>
class Point3
{
public:
	union
	{
		struct
		{
			T x, y, z;
		};
		struct
		{
			T width, height, depth;
		};
	};
	
	Point3(T x, T y, T z)
		: x(x), y(y), z(z)
	{}
	Point3()
		:
		Point3(T(0.0), T(0.0), T(0.0))
	{}
	template <class T2, class T3, class T4>
	Point3(T2 x, T3 y, T4 z)
		:
		x(T(x)),
		y(T(y)),
		z(T(z))
	{}
	template <class T2>
	Point3(const Point3<T2>& copy)
	{
		x = T(copy.x);
		y = T(copy.y);
		z = T(copy.z);
	}
	Point3(glm::vec3 v)
		:
		x(T(v.x)),
		y(T(v.y)),
		z(T(v.z))
	{}
	//operators
	inline bool operator==(const Point3<T>& rhs) const
	{
		if (x == rhs.x)
			if (y == rhs.y)
				if(z == rhs.z)
					return true;
		return false;
	}
	inline bool operator!=(const Point3<T>& rhs) const
	{
		return !(*this == rhs);
	}
	inline bool isEqual(const Point3<T>& other, T tolerance)
	{
		if (x >= other.x - tolerance)
			if (x <= other.x + tolerance)
				if (y >= other.y - tolerance)
					if (y <= other.y + tolerance)
						if (z >= other.z - tolerance)
							if (z <= other.z + tolerance)
								return true;
		return false;
	}
	template <class T2>
	inline Point3<T>& operator=(const Point3<T2>& rhs)
	{
		x = T(rhs.x);
		y = T(rhs.y);
		z = T(rhs.z);
		return *this;
	}
	inline Point3<T>& operator=(const Point3<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	inline Point3<T>& operator+=(const Point3<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	inline Point3<T>& operator-=(const Point3<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	inline Point3<T> operator+(const Point3<T>& rhs) const
	{
		Point3<T> copy = *this;
		return copy += rhs;
	}
	inline Point3<T> operator-(const Point3<T>& rhs) const
	{
		Point3<T> copy = *this;
		return copy -= rhs;
	}
	inline Point3<T> operator-() const
	{
		return Point3<T>(*this) * T(-1.0);
	}
	//vector
	inline Point3<T>& operator*=(const T& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		return *this;
	}
	inline Point3<T> operator*(const T& rhs) const
	{
		Point3<T> res = *this;
		return (res *= rhs);
	}
	inline Point3<T>& operator/=(const T& rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;
		return *this;
	}
	inline Point3<T> operator/(const T& rhs) const
	{
		Point3<T> res = *this;
		return (res /= rhs);
	}
	// dot product
	inline T operator*(const Point3<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	//operators concerning the length
	inline bool operator>(const Point3<T>& rhs)
	{
		return lengthSq() > rhs.lengthSq();
	}
	inline bool operator<(const Point3<T>& rhs)
	{
		return lengthSq() < rhs.lengthSq();
	}
	inline bool operator>=(const Point3<T>& rhs)
	{
		return lengthSq() >= rhs.lengthSq();
	}
	inline bool operator<=(const Point3<T>& rhs)
	{
		return lengthSq() <= rhs.lengthSq();
	}

	//helper functions
	inline T lengthSq() const
	{
		return x * x + y * y + z * z;
	}
	inline T length() const
	{
		return std::sqrt(lengthSq());
	}
	inline Point3<T> normalize() const
	{
		Point3<T> res(*this);
		T len = length();
		assert(len != T(0));
		if (len != T(0))
			return res /= len;
		else
			return res;
	}
	inline T size() const
	{
		return x * y * z;
	}
	inline glm::vec3 toGlmVec3() const
	{
		return glm::vec3(float(x), float(y), float(z));
	}

	T calcIndex(Point3 pos) const
	{
		assert(pos.x < width);
		assert(pos.y < height);
		assert(pos.z < depth);
		return width * (height * pos.z + pos.y) + pos.x;
	}
	Point3<T> fromIndex(T i) const
	{
		Point3 p;
		p.z = i / (width * height);
		p.y = (i % (width * height)) / width;
		p.x = (i % (width * height)) % width;
		return p;
	}
};

// min max func 
namespace std
{
	template<class T>
	Point3<T> min(const Point3<T>& a, const Point3<T>& b)
	{
		return Point3<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}

	template<class T>
	Point3<T> max(const Point3<T>& a, const Point3<T>& b)
	{
		return Point3<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}
}

using Point3F = Point3<float>;
using Point3I = Point3<int>;
using Point3S = Point3<size_t>;