// include this first in each test

#include <iostream>
#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#define EXPECT_REAL_EQ EXPECT_FLOAT_EQ
#define PWN_EXPECT_EQ(a,b) EXPECT_PRED_FORMAT2(::pwn::math::IsEqual, a, b)

#include <gtest/gtest.h>

namespace pwn
{
	namespace math
	{
		bool IsEqual(const vec2& d, const vec2& rhs);
		bool IsEqual(const point2& d, const point2& rhs);
		bool IsEqual(const vec3& d, const vec3& rhs);
		bool IsEqual(const quat& lhs, const quat& rhs);
		bool IsEqual(const mat33& lhs, const mat33& rhs);
		bool IsEqual(const mat44& lhs, const mat44& rhs);
		bool IsEqual(const rect& lhs, const rect& rhs);
		bool IsEqual(const AxisAngle& lhs, const AxisAngle& rhs);

		::testing::AssertionResult vec2_equal_test(const char* a, const char* b, const vec2& d, const vec2& rhs);
		::testing::AssertionResult point2_equal_test(const char* a, const char* b, const point2& d, const point2& rhs);
		::testing::AssertionResult vec3_equal_test(const char* a, const char* b, const vec3& d, const vec3& rhs);
		::testing::AssertionResult quat_equal_test(const char* a, const char* b, const quat& lhs, const quat& rhs);
		::testing::AssertionResult mat33_equal_test(const char* a, const char* b, const mat33& lhs, const mat33& rhs);
		::testing::AssertionResult mat44_equal_test(const char* a, const char* b, const mat44& lhs, const mat44& rhs);
		::testing::AssertionResult rect_equal_test(const char* a, const char* b, const rect& lhs, const rect& rhs);
		::testing::AssertionResult axisangle_equal_test(const char* a, const char* b, const AxisAngle& lhs, const AxisAngle& rhs);
	}
}


