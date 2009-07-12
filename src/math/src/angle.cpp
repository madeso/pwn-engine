#include <sml/angled>
#include <sml/math1>

namespace sml
{
	namespace // loval
	{
		const real rad2deg(const real rad)
		{
			return (180/Pi()) * rad;
		}
		const real deg2rad(const real deg)
		{
			return Pi()/180 * deg;
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
		mRad = Wrap(0, mRad, Pi()*2);
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
}