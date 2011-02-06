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
		point2::point2(real x, real y)
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

		// -------------------------------------------

		const point2 Origo2()
		{
			return point2(PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0));
		}

		const real DistanceBetween(const point2& from, const point2& to)
		{
			return LengthOf(FromTo(from, to));
		}

		const point2 ChangeY(const point2& v, const real newy)
		{
			return point2(v.vec.x, newy);
		}

		// -------------------------------------------

		const point2 operator+(const point2& lhs, const direction2& rhs)
		{
			return point2(lhs.vec+rhs.vec);
		}

		const point2 operator+(const direction2& lhs, const point2& rhs)
		{
			return point2(lhs.vec+rhs.vec);
		}

		const point2 operator-(const point2& lhs, const direction2& rhs)
		{
			return point2(lhs.vec-rhs.vec);
		}

		const point2 operator-(const direction2& lhs, const point2& rhs)
		{
			return point2(lhs.vec-rhs.vec);
		}

		const point2 operator*(const point2& lhs, const real rhs)
		{
			return point2(lhs.vec*rhs);
		}

		const point2 operator*(const real lhs, const point2& rhs)
		{
			return point2(lhs*rhs.vec);
		}
	}
}
