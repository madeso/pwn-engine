// include this first in each test

#include <iostream>
#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#define EXPECT_REAL_EQ EXPECT_FLOAT_EQ
#define PWN_EXPECT_EQ(a,b) EXPECT_PRED2(::pwn::math::IsEqual, a, b)
namespace pwn
{
	namespace math
	{
		bool vec2_equal(const vec2& d, const vec2& rhs);
		//std::ostream& operator<<(std::ostream& o, const vec2& t);

		bool point2_equal(const point2& d, const point2& rhs);
		std::ostream& operator<<(std::ostream& o, const point2& t);

		bool vec3_equal(const vec3& d, const vec3& rhs);
		//std::ostream& operator<<(std::ostream& o, const vec3& t);

		bool quat_equal(const quat& lhs, const quat& rhs);
		//std::ostream& operator<<(std::ostream& o, const quat& t);

		bool mat33_equal(const mat33& lhs, const mat33& rhs);
		//std::ostream& operator<<(std::ostream& o, const mat33& m);

		bool mat44_equal(const mat44& lhs, const mat44& rhs);
		//std::ostream& operator<<(std::ostream& o, const mat44& m);

		bool rect_equal(const rect& lhs, const rect& rhs);
		std::ostream& operator<<(std::ostream& o, const rect& aa);

		bool axisangle_equal(const AxisAngle& lhs, const AxisAngle& rhs);
		std::ostream& operator<<(std::ostream& o, const AxisAngle& aa);
	}
}

#include <gtest/gtest.h>
