#include <sml/axisangle>
#include <sml/vec3op>

namespace sml
{
	AxisAngle::AxisAngle(const vec3& ax, const Angle& ang)
		: axis( GetNormalized(ax) )
		, angle(ang)
	{
	}

	const AxisAngle RightHandAround(const vec3& axis, const Angle& angle)
	{
		return AxisAngle(axis, angle);
	}
}