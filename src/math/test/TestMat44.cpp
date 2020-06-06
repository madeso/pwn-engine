#include "TestBase.h"

#define GTEST(x) TEST(Mat44, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	GTEST(TestTransormation)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, cvec3(cmat44(vec3(1, 2, 3))), vec3(1, 2, 3));
	}


	GTEST(TestRight)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Right(mat44Identity()), Right());
	}


	GTEST(TestUp)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Up(mat44Identity()), Up());
	}


	GTEST(TestIn)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, In(mat44Identity()), In());
	}

	struct TestRotationFixture : ::testing::Test
	{
		mat44 start;
		AxisAngle aa;
		vec3 toTransform;
		vec3 result;

		TestRotationFixture()
			: start(mat44Identity())
			, aa(RightHandAround(Up(), Angle::FromDegrees(-90)))
			, toTransform(0, 0, -5)
			, result(5, 0, 0)
		{
		}
	};

	TEST_F(TestRotationFixture, TestRotationAxisAngle)
	{
		vec3 r = mat44helper(start)
		         .rotate(aa)
		         .transform(toTransform);
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test,  r, result);
	}

	TEST_F(TestRotationFixture, TestRotationQuat)
	{
		vec3 r = mat44helper(start)
		         .rotate(cquat(aa))
		         .transform(toTransform);
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test,  r, result);
	}

	GTEST(TestCombined_RT)
	{
		vec3 r = mat44helper(mat44Identity())
		         .rotate(RightHandAround(Up(), Angle::FromDegrees(-90)))
		         .translate(vec3(0, 0, -5))
		         .transform(vec3(0, 0, 0));
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, r, vec3(5, 0, 0));
	}

	GTEST(TestCombined2_RT)
	{
		vec3 r = mat44helper(mat44Identity())
		         .rotate(RightHandAround(Up(), Angle::FromDegrees(90)))
		         .translate(vec3(0, 0, -5))
		         .transform(vec3(0, 0, 0));
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, r, vec3(-5, 0, 0));
	}

	GTEST(TestCombined_TR)
	{
		vec3 r = mat44helper(mat44Identity())
		         .translate(vec3(0, 0, 5))
		         .rotate(RightHandAround(Up(), Angle::FromDegrees(-90)))
		         .transform(vec3(0, 0, 0));
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, r, vec3(0, 0, 5));
	}


	GTEST(TestTranslation)
	{
		vec3 r = mat44helper(mat44Identity())
		         .translate(vec3(1, 2, 3))
		         .transform(vec3(7, 8, 9));
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, r, vec3(8, 10, 12));
	}


	GTEST(TestIentityTransform)
	{
		vec3 r = mat44helper(mat44Identity())
		         .transform(vec3(1, 2, 3));
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, r, vec3(1, 2, 3));
	}


	GTEST(TestIentityMultiply)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::mat44_equal_test, mat44Identity() * mat44Identity(), mat44Identity());
	}
}
