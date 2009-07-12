#include <sml/angled>
#include <sml/angleop>
#include <cmath>
#include <sml/math1>

namespace sml
{
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
		return Angle::FromRadians(std::asin(v));
	}
	const Angle Acos(const real v)
	{
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
		return Angle::FromRadians(percent * Pi() * 2);
	}
	const Angle Zero()
	{
		return Angle::FromRadians(0);
	}
}