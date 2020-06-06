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
		bool IsEqual(real lhs, real rhs)
		{
			return IsZero(lhs - rhs);
		}

		bool IsZero(real r)
		{
			const real kRange = 0.0001f;
			return IsWithin(-kRange, r, kRange);
		}

		real ZeroOrValue(real r)
		{
			if(IsZero(r))
			{
				return 0;
			}
			else
			{
				return r;
			}
		}

		const int Sign(const real r)
		{
			if(r > PWN_MATH_VALUE(0.0))
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}

		const real Lerp(const real f, real scale, const real t)
		{
			return f + (t - f) * scale;
		}

		const real Curve(const real newValue, const real oldValue, const real smoothingValue)
		{
			const int sign = Sign(oldValue - newValue);
			const real slip = (oldValue - newValue) / smoothingValue;
			const real val = oldValue - slip;
			if(sign != Sign(val - newValue))
			{
				return newValue;
			}
			else
			{
				return val;
			}
		}

		const real Square(const real r)
		{
			return r * r;
		}

		const real Sqrt(const real r)
		{
			Assert(r >= 0 && "input must be bigger than 0");
			return std::sqrt(r);
		}

		const real Abs(const real r)
		{
			if(r < PWN_MATH_VALUE(0.0))
			{
				return -r;
			}
			else
			{
				return r;
			}
		}

		const real Min(const real lhs, const real rhs)
		{
			if(lhs < rhs)
			{
				return lhs;
			}
			else
			{
				return rhs;
			}
		}

		const real Max(const real lhs, const real rhs)
		{
			if(lhs > rhs)
			{
				return lhs;
			}
			else
			{
				return rhs;
			}
		}

		const real To01(const real L, const real v, const real U)
		{
			return (v - L) / (U - L);
		}

		const real From01(const real L, const real v, const real U)
		{
			return v * (U - L) + L;
		}

		const real Remap(const real ol, const real ou, const real v, const real nl, const real nu)
		{
			return From01(nl, To01(ol, v, ou), nu);
		}

		const real Get360Angular(const real min, const real value, const real max)
		{
			const real halfdiff = (max - min) / PWN_MATH_VALUE(2.0);
			return min + halfdiff - halfdiff * Cos(Angle::FromDegrees(value * PWN_MATH_VALUE(360.0)));
		}

		const real KeepWithin(const real min, const real v, const real max)
		{
			if(v > max)
			{
				return max;
			}
			else if(v < min)
			{
				return min;
			}
			else
			{
				return v;
			}
		}

		const bool IsWithin(const real min, const real c, const real max)
		{
			return c > min && c < max;
		}

		const bool IsWithinInclusive(const real min, const real c, const real max)
		{
			return c >= min && c <= max;
		}

		const real Wrap(const real min, const real v, const real max)
		{
			const real diff = max - min;
			Assert(diff > 0);
			real value = v - min;
			while(value < PWN_MATH_VALUE(0.0))
			{
				value += diff;
			}
			while(value > diff)
			{
				value -= diff;
			}
			return min + value;
		}

		const int Wrapi(const int min, const int v, const int max)
		{
			const int diff = max - min + 1;
			int value = v;
			while(value < min)
			{
				value += diff;
			}
			while(value > max)
			{
				value -= diff;
			}
			return value;
		}

		const int IncrementAndWrap(const real min, real* current, const real change, const real max)
		{
			const real diff = max - min;
			real value = *current + change;
			int wraps = 0;
			while(value < min)
			{
				value += diff;
				--wraps;
			}
			while(value > max)
			{
				value -= diff;
				++wraps;
			}
			*current = value;
			return wraps;
		}

		const int IncrementAndWrapi(const int min, int* current, const int change, const int max)
		{
			const int diff = max - min + 1;
			int value = *current + change;
			int wraps = 0;
			while(value < min)
			{
				value += diff;
				--wraps;
			}
			while(value > max)
			{
				value -= diff;
				++wraps;
			}
			*current = value;
			return wraps;
		}

		namespace // internal
		{
			real LowerBound(real num, real gran)
			{
				return std::floor(num / gran) * gran;
			}

			real UpperBound(real num, real gran)
			{
				return std::ceil(num / gran) * gran;
			}
		}

		const real Round(const real num, const real gran)
		{
			const real lower = LowerBound(num, gran);
			const real upper = UpperBound(num, gran);
			const real percent = (num - lower) / gran; // 1.0 is upper, 0.0 is lower, 0.25 is 25% between lower and upper, moving from lower to upper

			if(percent >= 0.5)
			{
				return upper;
			}
			else
			{
				return lower;
			}
		}

		const real Pi()
		{
			return PWN_MATH_VALUE(3.1415926535897932384626433832795);
		}

		const real HalfPi()
		{
			return Pi() / PWN_MATH_VALUE(2.0);
		}

		const bool kTrue()
		{
			return true;
		}

		const bool kFalse()
		{
			return false;
		}

		/*

		ArcBall::ArcBall(const vec2& center, const real radius)
		: center(center)
		, radius(radius)
		{
		}

		// code found somewhere that seemed to be working, is this correct?

		namespace // local
		{
		vec3 PointOnBall(const ArcBall& ab, const vec2& v)
		{
		const real x = ( v.x - ab.center.x ) / ab.radius;
		const real y = ( v.y - ab.center.y ) / ab.radius;
		const real rsq = Square(x) + Square(y);
		const real z = rsq < PWN_MATH_VALUE(1.0) ? Sqrt(PWN_MATH_VALUE(1.0) - rsq) : PWN_MATH_VALUE(0.0);
		return GetNormalized(vec3(x,y,z));
		}
		}

		quat GetRotation(const ArcBall& arc, const vec2 from, const vec2 to)
		{
		const vec3 f = PointOnBall(arc, from);
		const vec3 t = PointOnBall(arc, to);
		const vec3 axis = crossNorm(f,t);
		const Angle angle = AngleBetween(f,t);
		return cquat(RightHandAround(axis, angle));
		}

		*/
	}
}
