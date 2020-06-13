#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

namespace pwn
{
namespace math
{
    const vec3
    In(const mat44& m)
    {
        return In(cmat33(m));
    }
    const vec3
    Right(const mat44& m)
    {
        return Right(cmat33(m));
    }
    const vec3
    Up(const mat44& m)
    {
        return Up(cmat33(m));
    }

    const vec3
    Out(const mat44& m)
    {
        return Out(cmat33(m));
    }
    const vec3
    Left(const mat44& m)
    {
        return Left(cmat33(m));
    }
    const vec3
    Down(const mat44& m)
    {
        return Down(cmat33(m));
    }

    const mat44
    mat44_FromColMajor(
            const real data0,
            const real data1,
            const real data2,
            const real data3,
            const real data4,
            const real data5,
            const real data6,
            const real data7,
            const real data8,
            const real data9,
            const real data10,
            const real data11,
            const real data12,
            const real data13,
            const real data14,
            const real data15)
    {
        return mat44(
                data0,
                data4,
                data8,
                data12,
                data1,
                data5,
                data9,
                data13,
                data2,
                data6,
                data10,
                data14,
                data3,
                data7,
                data11,
                data15);
    }

    const mat44
    cmat44(const mat33& m)
    {
        return mat44(
                m(0, 0),
                m(0, 1),
                m(0, 2),
                0,
                m(1, 0),
                m(1, 1),
                m(1, 2),
                0,
                m(2, 0),
                m(2, 1),
                m(2, 2),
                0,
                0,
                0,
                0,
                1);
    }

    const vec3
    operator*(const mat44& m, const vec3& v)
    {
        const vec4 t = m * vec4(v[0], v[1], v[2], 1);
        return vec3(t[0], t[1], t[2]);
    }

    const mat44
    SetTransform(const mat44& m, const vec3& t)
    {
        mat44 tmp = m;
        cml::matrix_set_translation(tmp, t);
        return tmp;
    }

    const mat44
    cmat44(const AxisAngle& aa)
    {
        mat44 m;
        cml::matrix_rotation_axis_angle(m, aa.axis, aa.angle.inRadians());
        return m;
    }

    const mat44
    cmat44(const quat& q)
    {
        mat44 m;
        cml::matrix_rotation_quaternion(m, q);
        return m;
    }

    const mat44
    mat44Identity()
    {
        return cml::identity<4>();
    }

    const mat44
    cmat44(const vec3& v)
    {
        mat44 m = mat44Identity();
        cml::matrix_set_translation(m, v);
        return m;
    }

    vec3
    TranslateWithInverseMatrix(const vec3 vec, const mat44& mat)
    {
        return cml::inverse(mat) * vec;
    }

    mat44helper::mat44helper(const mat44& mat) : mat(mat)
    {
    }
    mat44helper&
    mat44helper::mult(const mat44& m)
    {
        mat = mat * m;
        return *this;
    }
    mat44helper&
    mat44helper::rotate(const AxisAngle& aa)
    {
        return mult(cmat44(aa));
    }

    mat44helper&
    mat44helper::rotate(const quat& q)
    {
        return mult(cmat44(q));
    }
    mat44helper&
    mat44helper::translate(const vec3& t)
    {
        return mult(cmat44(t));
    }
    const vec3
    mat44helper::transform(const vec3& v) const
    {
        return mat * v;
    }
}
}
