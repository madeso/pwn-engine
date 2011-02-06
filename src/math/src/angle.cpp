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
		namespace // local
		{
			const real rad2deg(const real rad)
			{
				return (PWN_MATH_VALUE(180.0)/Pi()) * rad;
			}
			const real deg2rad(const real deg)
			{
				return Pi()/PWN_MATH_VALUE(180.0) * deg;
			}
		}

		const real Angle::inDegrees() const
		{
			return rad2deg(mRad);
		}

		const real Angle::inRadians() const
		{
			return mRad;
		}

		const Angle Angle::FromDegrees(real deg)
		{
			return Angle(deg2rad(deg));
		}

		const Angle Angle::FromRadians(real rad)
		{
			return Angle(rad);
		}

		Angle::Angle(real rad)
			: mRad(rad)
		{
		}

		void Angle::wrap()
		{
			mRad = Wrap(0, mRad, Pi()*PWN_MATH_VALUE(2.0));
		}

		void Angle::operator+=(const Angle& rhs)
		{
			mRad += rhs.mRad;
		}

		void Angle::operator-=(const Angle& rhs)
		{
			mRad -= rhs.mRad;
		}

		void Angle::operator *=(const real rhs)
		{
			mRad *= rhs;
		}

		const real Sin(const Angle& ang)
		{
			return std::sin(ang.inRadians());
		}

		const real Cos(const Angle& ang)
		{
			return std::cos(ang.inRadians());
		}

		const real Tan(const Angle& ang)
		{
			return std::tan(ang.inRadians());
		}

		const Angle Asin(const real v)
		{
		    Assert(v <= 1 && "v must be smaller than 1, use Limmit or Max on the value to not trigger this Assert");
			Assert(v >= -1 && "v must be greater than -1, use Limmit or Min on the value to not trigger this Assert");
			return Angle::FromRadians(std::asin(v));
		}

		const Angle Acos(const real v)
		{
			Assert(v <= 1 && "v must be smaller than 1, use Limmit or Max on the value to not trigger this Assert");
			Assert(v >= -1 && "v must be greater than -1, use Limmit or Min on the value to not trigger this Assert");
			return Angle::FromRadians(std::acos(v));
		}

		const Angle Atan(const real v)
		{
			return Angle::FromRadians(std::atan(v));
		}

		const Angle GetWrapped(const Angle& a)
		{
			Angle temp(a);
			temp.wrap();
			return temp;
		}

		const Angle operator+(const Angle& lhs, const Angle& rhs)
		{
			Angle temp(lhs);
			temp+=rhs;
			return temp;
		}

		const Angle operator-(const Angle& lhs, const Angle& rhs)
		{
			Angle temp(lhs);
			temp-=rhs;
			return temp;
		}

		const Angle operator*(const Angle& lhs, const real rhs)
		{
			Angle temp(lhs);
			temp*=rhs;
			return temp;
		}

		const Angle operator*(const real rhs, const Angle& lhs)
		{
			return lhs*rhs;
		}

		const Angle FromPercentOf360(const real percent)
		{
			return Angle::FromRadians(percent * Pi() * PWN_MATH_VALUE(2.0));
		}

		const Angle Zero()
		{
			return Angle::FromRadians(0);
		}
	}
}
