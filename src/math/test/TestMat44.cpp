#include "TestBase.h"

SUITE(test_mat44)
{
	using namespace pwn::math;

	TEST(TestTransormation)
	{
		CHECK_EQUAL(cvec3(cmat44(vec3(1, 2, 3))), vec3(1, 2, 3));
	}

	
	TEST(TestRight)
	{
		CHECK_EQUAL(Right(mat44Identity()), Right());
	}

	
	TEST(TestUp)
	{
		CHECK_EQUAL(Up(mat44Identity()), Up());
	}

	
	TEST(TestIn)
	{
		CHECK_EQUAL(In(mat44Identity()), In());
	}

	struct TestRotationFixture
	{
		mat44 start;
		AxisAngle aa;
		vec3 toTransform;
		vec3 result;

		TestRotationFixture()
			: start(mat44Identity())
			, aa( RightHandAround(Up(), Angle::FromDegrees(-90)) )
			, toTransform(0,0,5)
			, result(5,0,0)
		{
		}
	};
	
	TEST_FIXTURE(TestRotationFixture, TestRotationAxisAngle)
	{
		vec3 r = mat44helper(start)
			.rotate( aa )
			.transform( toTransform );
		CHECK_EQUAL( r, result);
	}

	TEST_FIXTURE(TestRotationFixture, TestRotationQuat)
	{
		vec3 r = mat44helper(start)
			.rotate( cquat(aa) )
			.transform( toTransform );
		CHECK_EQUAL( r, result);
	}

	
	TEST(TestCombined)
	{
		vec3 r = mat44helper(mat44Identity())
			.rotate(RightHandAround(Up(), Angle::FromDegrees(-90)))
			.translate(vec3(0, 0, 5))
			.transform(vec3(0, 0, 0));
		CHECK_EQUAL(r, vec3(5, 0, 0));
	}

	
	TEST(TestTranslation)
	{
		vec3 r = mat44helper(mat44Identity())
			.translate(vec3(1,2,3))
			.transform(vec3(7, 8, 9));
		CHECK_EQUAL(r, vec3(8, 10, 12));
	}

	
	TEST(TestIentityTransform)
	{
		vec3 r = mat44helper(mat44Identity())
			.transform(vec3(1, 2, 3));
		CHECK_EQUAL(r, vec3(1, 2, 3));
	}

	
	TEST(TestIentityMultiply)
	{
		CHECK_EQUAL(mat44Identity() * mat44Identity(), mat44Identity());
	}
}