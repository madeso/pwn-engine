#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

namespace pwn
{
	namespace math
	{
		const vec2 GetNormalized(const vec2& vec)
		{
			if( vec.length_squared() < 0.001f ) return vec;
			vec2 temp = vec;
			temp.normalize();
			return temp;
		}

		const vec2 FromTo(const vec2& from, const vec2& to)
		{
			return to - from;
		}

		const vec2 Curve(const vec2& target, const vec2& old, real smoothing)
		{
			return vec2(Curve(X(target), X(old), smoothing), Curve(Y(target), Y(old), smoothing));
		}

		const vec2 ChangeY(const vec2& v, const real newy)
		{
			return vec2(X(v), newy);
		}
	}
}
