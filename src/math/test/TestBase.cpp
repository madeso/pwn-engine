#include "TestBase.h"

namespace  // local
{
using namespace pwn;
using namespace pwn::math;
bool
equal(real lhs, real rhs)
{
    return Abs(lhs - rhs) < 0.0001;
}
}

namespace pwn
{
namespace math
{
    bool
    IsEqual(const vec2& d, const vec2& rhs)
    {
        return equal(math::X(d), math::X(rhs)) &&
                equal(math::Y(d), math::Y(rhs));
    }

    bool
    IsEqual(const point2& d, const point2& rhs)
    {
        return equal(d.x(), rhs.x()) && equal(d.y(), rhs.y());
    }

    bool
    IsEqual(const vec3& d, const vec3& rhs)
    {
        return equal(math::X(d), math::X(rhs)) &&
                equal(math::Y(d), math::Y(rhs)) &&
                equal(math::Z(d), math::Z(rhs));
    }


    bool
    IsEqual(const quat& lhs, const quat& rhs)
    {
        return equal(
                cml::dot(lhs, rhs),
                1);  //, math::X(rhs)) && equal(math::Y(lhs), math::Y(rhs)) && equal(math::Z(lhs), math::Z(rhs)) && equal(math::W(lhs), math::W(rhs));
    }


    bool
    IsEqual(const mat33& lhs, const mat33& rhs)
    {
        for (int i = 0; i < 3 * 3; ++i)
            if (!equal(lhs.data()[i], rhs.data()[i]))
            {
                return false;
            }
        return true;
    }

    bool
    IsEqual(const mat44& lhs, const mat44& rhs)
    {
        for (int i = 0; i < 4 * 4; ++i)
            if (!equal(lhs.data()[i], rhs.data()[i]))
            {
                return false;
            }
        return true;
    }


    bool
    IsEqual(const rect& lhs, const rect& rhs)
    {
        return IsEqual(lhs.upperLeft, rhs.upperLeft) &&
                IsEqual(lhs.lowerRight, rhs.lowerRight);
    }


    bool
    IsEqual(const AxisAngle& lhs, const AxisAngle& rhs)
    {
        if (equal(lhs.angle.inDegrees(), rhs.angle.inDegrees()) &&
            equal(lhs.angle.inDegrees(), 0))
        {
            return true;  // zero rotation is always equal zero
        }

        const bool a =
                (rhs.axis == lhs.axis &&
                 equal(rhs.angle.inDegrees(), lhs.angle.inDegrees()));
        const bool inv =
                (rhs.axis == -lhs.axis &&
                 equal(rhs.angle.inDegrees(), -lhs.angle.inDegrees()));
        return a || inv;
    }
    /*
::testing::AssertionResult&
    operator<<(::testing::AssertionResult& o, const vec2& t)
    {
        o << "(" << math::X(t) << ", " << math::Y(t) << ")";
        return o;
    }

    ::testing::AssertionResult&
    operator<<(::testing::AssertionResult& o, const point2& t)
    {
        o << "(" << t.x() << ", " << t.y() << ")";
        return o;
    }

    
    ::testing::AssertionResult&
    operator<<(::testing::AssertionResult& o, const vec3& t)
    {
        o << "(" << math::X(t) << ", " << math::Y(t) << ", " << math::Z(t)
          << ")";
        return o;
    }
    ::testing::AssertionResult&
    operator<<(::testing::AssertionResult& o, const quat& t)
    {
        std::stringstream ss;
        ss.precision(5);
        ss.setf(std::ios::fixed, std::ios::floatfield);
        ss << "(" << math::X(t) << ", " << math::Y(t) << ", " << math::Z(t)
           << ", " << math::W(t) << ")";
        o << ss.str();
        return o;
    }
    ::testing::AssertionResult&
    operator<<(::testing::AssertionResult& o, const mat33& m)
    {
        o << "( ";
        for (int i = 0; i < 3 * 3; ++i)
        {
            o << m.data()[i] << " ";
        }
        o << ")";
        return o;
    }
    ::testing::AssertionResult&
    operator<<(::testing::AssertionResult& o, const mat44& m)
    {
        o << "( ";
        for (int i = 0; i < 4 * 4; ++i)
        {
            o << m.data()[i] << " ";
        }
        o << ")";
        return o;
    }
    ::testing::AssertionResult&
    operator<<(::testing::AssertionResult& o, const rect& t)
    {
        o << "(" << t.upperLeft.x() << ", " << t.upperLeft.y() << ", "
          << t.lowerRight.x() << ", " << t.lowerRight.y() << ")";
        return o;
    }
    ::testing::AssertionResult&
    operator<<(::testing::AssertionResult& o, const AxisAngle& aa)
    {
        o << "(" << math::X(aa.axis) << ", " << math::Y(aa.axis) << ", "
          << math::Z(aa.axis) << ": " << aa.angle.inDegrees() << ")";
        return o;
    }

    template <typename T>
    ::testing::AssertionResult
    equal_test(const char* aa, const char* bb, const T& a, const T& b)
    {
        if (IsEqual(a, b))
        {
            return ::testing::AssertionSuccess();
        }
        else
        {
            return ::testing::AssertionFailure() << "Not equal! Expected:\n"
                                                 << a << " but got:\n"
                                                 << b;
        }
    }

    ::testing::AssertionResult
    vec2_equal_test(
            const char* a,
            const char* b,
            const vec2& d,
            const vec2& rhs)
    {
        return equal_test(a, b, d, rhs);
    }
    ::testing::AssertionResult
    point2_equal_test(
            const char* a,
            const char* b,
            const point2& d,
            const point2& rhs)
    {
        return equal_test(a, b, d, rhs);
    }
    ::testing::AssertionResult
    vec3_equal_test(
            const char* a,
            const char* b,
            const vec3& d,
            const vec3& rhs)
    {
        return equal_test(a, b, d, rhs);
    }
    ::testing::AssertionResult
    quat_equal_test(
            const char* aa,
            const char* bb,
            const quat& a,
            const quat& b)
    {
        if (IsEqual(a, b))
        {
            return ::testing::AssertionSuccess();
        }
        else
        {
            return ::testing::AssertionFailure() << "Not equal! Expected:\n"
                                                 << a << " but got:\n"
                                                 << b << "\n"
                                                 << "Dot is " << cml::dot(a, b);
        }
    }
    ::testing::AssertionResult
    mat33_equal_test(
            const char* a,
            const char* b,
            const mat33& lhs,
            const mat33& rhs)
    {
        return equal_test(a, b, lhs, rhs);
    }
    ::testing::AssertionResult
    mat44_equal_test(
            const char* a,
            const char* b,
            const mat44& lhs,
            const mat44& rhs)
    {
        return equal_test(a, b, lhs, rhs);
    }
    ::testing::AssertionResult
    rect_equal_test(
            const char* a,
            const char* b,
            const rect& lhs,
            const rect& rhs)
    {
        return equal_test(a, b, lhs, rhs);
    }
    ::testing::AssertionResult
    axisangle_equal_test(
            const char* a,
            const char* b,
            const AxisAngle& lhs,
            const AxisAngle& rhs)
    {
        return equal_test(a, b, lhs, rhs);
    }
    */
}
}
