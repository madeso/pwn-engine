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
		direction3::direction3(real x, real y, real z)
			: vec(x, y, z)
		{
		}

		direction3::direction3(const vec3& data)
			: vec(data)
		{
		}

		void direction3::operator+=(const direction3& rhs)
		{
			vec += rhs.vec;
		}

		// ------------------------------------------------------------------------

		const real LengthOf(const direction3& dir)
		{
			return dir.vec.length();
		}
		const direction3 ToUnit(const direction3& dir)
		{
			return direction3( GetNormalized(dir.vec) );
		}

		const direction3 FromTo(const point3& from, const point3& to)
		{
			return direction3( to.vec-from.vec );
		}

		// ------------------------------------------------------------------------

		const direction3 operator+(const direction3& lhs, const direction3& rhs)
		{
			return direction3(lhs.vec+rhs.vec);
		}

		const direction3 operator-(const direction3& lhs, const direction3& rhs)
		{
			return direction3(lhs.vec-rhs.vec);
		}

		const direction3 operator*(const direction3& lhs, const real rhs)
		{
			return direction3(lhs.vec*rhs);
		}
		const direction3 operator*(const real lhs, const direction3& rhs)
		{
			return direction3(lhs*rhs.vec);
		}
		const direction3 operator/(const direction3& lhs, const real rhs)
		{
			return direction3(lhs.vec / rhs);
		}
	}
}
