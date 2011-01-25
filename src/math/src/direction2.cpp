#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

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
		direction2::direction2(real x, real y)
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

		// ------------------------------------------------------------------------

		const real LengthOf(const direction2& dir)
		{
			return LengthOf(dir.vec);
		}
		const direction2 ToUnit(const direction2& dir)
		{
			return direction2( GetNormalized(dir.vec) );
		}

		const direction2 FromTo(const point2& from, const point2& to)
		{
			return direction2( to.vec-from.vec );
		}

		// ------------------------------------------------------------------------

		const direction2 operator+(const direction2& lhs, const direction2& rhs)
		{
			return direction2(lhs.vec+rhs.vec);
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
		const direction2 operator/(const direction2& lhs, const real rhs)
		{
			return direction2(lhs.vec / rhs);
		}
	}
}
