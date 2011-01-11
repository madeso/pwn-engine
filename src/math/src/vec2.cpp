#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/Assert.h>

#ifdef PWN_USE_CUSTOM_MATH
#include <cmath>
#include <memory> // memcpy
#else
#include <cml/cml.h>
#endif

namespace pwn
{
	namespace math
	{
		vec2::vec2(real ax, real ay)
			: x(ax)
			, y(ay)
		{
		}

		vec2::vec2()
			: x(0)
			, y(0)
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

		// -----------------------------------------

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

		const vec2 FromTo(const vec2& from, const vec2& to)
		{
			return to - from;
		}

		const vec2 Curve(const vec2& target, const vec2& old, real smoothing)
		{
			return vec2(Curve(target.x, old.x, smoothing), Curve(target.y, old.y, smoothing));
		}

		const vec2 ChangeY(const vec2& v, const real newy)
		{
			return vec2(v.x, newy);
		}

		// ---------------------------------------------------

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
}