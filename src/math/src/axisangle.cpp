#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

#ifdef PWN_USE_CUSTOM_MATH
#include <cmath>
#include <memory>  // memcpy
#else
#include <cml/cml.h>
#endif

namespace pwn
{
namespace math
{
    AxisAngle::AxisAngle(const vec3& ax, const Angle& ang)
        : axis(GetNormalized(ax))
        , angle(ang)
    {
    }

    // rewrite to better fit the mathematics instead of this "hack"
    const AxisAngle
    cAxisAngle(const quat& q)
    {
        Assert(W(q) <=
               1);  // if this happes, we should normalize, but this shouldnt happen
        vec3 axis;
        real angle;
        cml::quaternion_to_axis_angle(q, axis, angle);
        AxisAngle aa(axis, Angle::FromRadians(angle));

        return aa;
    }

    const AxisAngle
    RightHandAround(const vec3& axis, const Angle& angle)
    {
        return AxisAngle(axis, Angle::FromRadians(-angle.inRadians()));
    }
}
}
