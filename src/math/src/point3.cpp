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
		point3::point3(real x, real y, real z)
			: vec(x, y, z)
		{
		}
		point3::point3(const vec3& data)
			: vec(data)
		{
		}
		const real point3::x() const
		{
			return vec.x;
		}
		const real point3::y() const
		{
			return vec.y;
		}
		void point3::operator+=(const direction3& dir)
		{
			vec += dir.vec;
		}

		// -------------------------------------------

		const point3 Origo3()
		{
			return point3(PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0),PWN_MATH_VALUE(0.0));
		}

		const real DistanceBetween(const point3& from, const point3& to)
		{
			return LengthOf(FromTo(from, to));
		}

		const point3 ChangeY(const point3& v, const real newy)
		{
			return point3(v.vec.x, newy, v.vec.z);
		}

		// -------------------------------------------

		const point3 operator+(const point3& lhs, const direction3& rhs)
		{
			return point3(lhs.vec+rhs.vec);
		}

		const point3 operator+(const direction3& lhs, const point3& rhs)
		{
			return point3(lhs.vec+rhs.vec);
		}

		const point3 operator-(const point3& lhs, const direction3& rhs)
		{
			return point3(lhs.vec-rhs.vec);
		}

		const point3 operator-(const direction3& lhs, const point3& rhs)
		{
			return point3(lhs.vec-rhs.vec);
		}

		const point3 operator*(const point3& lhs, const real rhs)
		{
			return point3(lhs.vec*rhs);
		}

		const point3 operator*(const real lhs, const point3& rhs)
		{
			return point3(lhs*rhs.vec);
		}
	}
}
