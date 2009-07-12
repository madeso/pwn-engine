#include <sml/vec2>
#include <sml/math1>
#include <cml/cml.h>

namespace sml
{
	/*namespace // local
	{
		typedef cml::vector< real, cml::external<2> > tvec;
		typedef cml::vector< const real, cml::external<2> > tcvec;
		tvec c(vec2* vec)
		{
			return tvec( vec->data);
		}
		const tcvec c(const vec2* vec)
		{
			return tcvec( vec->data);
		}
		const vec2 c(const tvec& vec)
		{
			return vec2(vec.
		}
	}*/

	vec2::vec2(real ax, real ay)
		: x(ax)
		, y(ay)
	{
	}

	real* vec2::data()
	{
		return &x;
	}

	const real* vec2::data() const
	{
		return &x;
	}

	real& vec2::operator[](int index)
	{
		if( index == 0 ) return x;
		else return y;
	}
	const real vec2::operator[](int index) const
	{
		if( index == 0 ) return x;
		else return y;
	}

	void vec2::operator+=(const vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	void vec2::operator-=(const vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}

	void vec2::operator*=(const real rhs)
	{
		x *= rhs;
		y *= rhs;
	}

	void vec2::operator/=(const real rhs)
	{
		x /= rhs;
		y /= rhs;
	}

	real LengthOf(const vec2& vec)
	{
		//return c(&vec).length();
		return Sqrt(LengthOfSquared(vec));
	}

	real LengthOfSquared(const vec2& vec)
	{
		return Square(vec.x) + Square(vec.y);
	}

	const vec2 GetNormalized(const vec2& vec)
	{
		vec2 temp(vec);
		Normalize(&temp);
		return temp;
	}
	void Normalize(vec2* vec)
	{
		*vec /= LengthOf(*vec);
	}

	const vec2 operator+(const vec2& lhs, const vec2& rhs)
	{
		vec2 temp(lhs);
		temp+= rhs;
		return temp;
	}
	const vec2 operator-(const vec2& lhs, const vec2& rhs)
	{
		vec2 temp(lhs);
		temp-= rhs;
		return temp;
	}

	const vec2 operator*(const real scalar, const vec2& vec)
	{
		vec2 temp(vec);
		temp *= scalar;
		return temp;
	}
	const vec2 operator*(const vec2& vec, const real scalar)
	{
		vec2 temp(vec);
		temp *= scalar;
		return temp;
	}
	const vec2 operator/(const vec2& vec, const real scalar)
	{
		vec2 temp(vec);
		temp /= scalar;
		return temp;
	}
	const vec2 operator-(const vec2& vec)
	{
		return vec2(-vec.x, -vec.y);
	}
}