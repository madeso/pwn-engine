#include <sml/pointdir2>
#include <sml/vec2op>

namespace sml
{
	point2::point2(float x, float y)
		: vec(x, y)
	{
	}
	point2::point2(const vec2& data)
		: vec(data)
	{
	}
	const real point2::x() const
	{
		return vec.x;
	}
	const real point2::y() const
	{
		return vec.y;
	}
	void point2::operator+=(const direction2& dir)
	{
		vec += dir.vec;
	}

	direction2::direction2(float x, float y)
		: vec(x, y)
	{
	}
	direction2::direction2(const vec2& data)
		: vec(data)
	{
	}
	void direction2::operator+=(const direction2& rhs)
	{
		vec += rhs.vec;
	}

	const point2 Origo2()
	{
		return point2(0,0);
	}

	const point2 operator+(const point2& lhs, const direction2& rhs)
	{
		return point2(lhs.vec+rhs.vec);
	}
	const point2 operator+(const direction2& lhs, const point2& rhs)
	{
		return point2(lhs.vec+rhs.vec);
	}
	const direction2 operator+(const direction2& lhs, const direction2& rhs)
	{
		return direction2(lhs.vec+rhs.vec);
	}

	const point2 operator-(const point2& lhs, const direction2& rhs)
	{
		return point2(lhs.vec-rhs.vec);
	}
	const point2 operator-(const direction2& lhs, const point2& rhs)
	{
		return point2(lhs.vec-rhs.vec);
	}
	const direction2 operator-(const direction2& lhs, const direction2& rhs)
	{
		return direction2(lhs.vec-rhs.vec);
	}

	const direction2 operator*(const direction2& lhs, const real rhs)
	{
		return direction2(lhs.vec*rhs);
	}
	const direction2 operator*(const real lhs, const direction2& rhs)
	{
		return direction2(lhs*rhs.vec);
	}

	const point2 operator*(const point2& lhs, const real rhs)
	{
		return point2(lhs.vec*rhs);
	}
	const point2 operator*(const real lhs, const point2& rhs)
	{
		return point2(lhs*rhs.vec);
	}

	const direction2 operator/(const direction2& lhs, const real rhs)
	{
		return direction2(lhs.vec / rhs);
	}

	const real LengthOf(const direction2& dir)
	{
		return LengthOf(dir.vec);
	}
	const direction2 ToUnit(const direction2& dir)
	{
		return direction2( GetNormalized(dir.vec) );
	}

	const direction2 Between(const point2& from, const point2& to)
	{
		return direction2( to.vec-from.vec );
	}
	const float DistanceBetween(const point2& from, const point2& to)
	{
		return LengthOf(Between(from, to));
	}
}