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
		AxisAngle::AxisAngle(const vec3& ax, const Angle& ang)
			: axis( GetNormalized(ax) )
			, angle(ang)
		{
		}

		// rewrite to better fit the mathematics instead of this "hack"
		const AxisAngle cAxisAngle(const quat& q)
		{
			Assert( q.w <= 1 ); // if this happes, we should normalize, but this shouldnt happen
			const vec3 axis = cvec3(q);
			AxisAngle aa(axis, Acos(q.w)*PWN_MATH_VALUE(2.0));

			return aa;
		}

		const AxisAngle RightHandAround(const vec3& axis, const Angle& angle)
		{
			return AxisAngle(axis, angle);
		}
	}
}
