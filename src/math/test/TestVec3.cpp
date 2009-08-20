#include "TestBase.h"

SUITE(Vector3d)
{
	using namespace pwn::math;

	TEST(testLengthOf)
	{
		CHECK_EQUAL(10, LengthOf(vec3(10, 0, 0)));
		CHECK_EQUAL(20, LengthOf(vec3(0, 20, 0)));
		CHECK_EQUAL(30, LengthOf(vec3(0, 0, 30)));
		CHECK_EQUAL(0, LengthOf(vec3(0, 0, 0)));
	}

	TEST(testArrayOp)
	{
		const vec3 ctest(7,9, 4);
		CHECK_EQUAL(7, ctest.data()[0]);
		CHECK_EQUAL(9, ctest.data()[1]);
		CHECK_EQUAL(4, ctest.data()[2]);
		CHECK_EQUAL(7, ctest[0]);
		CHECK_EQUAL(9, ctest[1]);
		CHECK_EQUAL(4, ctest[2]);
		CHECK_EQUAL(7, ctest.x);
		CHECK_EQUAL(9, ctest.y);
		CHECK_EQUAL(4, ctest.z);

		vec3 test(4,5, 6);
		CHECK_EQUAL(4, test.data()[0]);
		CHECK_EQUAL(5, test.data()[1]);
		CHECK_EQUAL(6, test.data()[2]);
		CHECK_EQUAL(4, test[0]);
		CHECK_EQUAL(5, test[1]);
		CHECK_EQUAL(6, test[2]);
		CHECK_EQUAL(4, test.x);
		CHECK_EQUAL(5, test.y);
		CHECK_EQUAL(6, test.z);
	}

	TEST(testAdd)
	{
		CHECK_EQUAL(vec3(4,4,4), vec3(1,2,3) + vec3(3,2,1));
		CHECK_EQUAL(vec3(1,2,3), vec3(-1,1,4) + vec3(2,1,-1));
	}

	TEST(testSub)
	{
		CHECK_EQUAL(vec3(0,1,0), vec3(0,2,0) - vec3(0,1,0));
		CHECK_EQUAL(vec3(-2,0,2), vec3(1,2,3) - vec3(3,2,1));
	}

	TEST(testDiv)
	{
		CHECK_EQUAL(vec3(4,2,1), vec3(8,4,2) / 2);
		CHECK_EQUAL(vec3(5,2,1), vec3(25,10,5) / 5);
	}

	TEST(testMult)
	{
		CHECK_EQUAL(vec3(8,4,2), vec3(4,2,1) * 2);
		CHECK_EQUAL(vec3(5,25,50), 5 * vec3(1,5,10));
	}

	TEST(testNormal)
	{
		CHECK_EQUAL(vec3(1,0,0), GetNormalized(vec3(4,0,0) ));
		CHECK_EQUAL(vec3(0,1,0), GetNormalized(vec3(0,9,0) ));
	}

	TEST(testDot)
	{
		CHECK_EQUAL(0, dot(vec3(1,0,0), vec3(0,1,0) ));
		CHECK_EQUAL(1, dot(vec3(1,0,0), vec3(1,0,0) ));
	}

	TEST(testCross)
	{
		CHECK_EQUAL(vec3(0,1,0), cross(vec3(0,0,-1), vec3(-1,0,0) ));
	}

	TEST(testAngle)
	{
		// Angle between 2, 0-180
		CHECK_EQUAL(0, AngleBetween(vec3(1,0,0), vec3(1,0,0)).inDegrees() );
		CHECK_EQUAL(90, AngleBetween(vec3(1,0,0), vec3(0,1,0)).inDegrees() );
		CHECK_EQUAL(180, AngleBetween(vec3(1,0,0), vec3(-1,0,0)).inDegrees() );
		CHECK_EQUAL(90, AngleBetween(vec3(1,0,0), vec3(0,-1,0)).inDegrees() );

		// specify extra - 0-360
		CHECK_EQUAL(0, AngleBetween(vec3(1,0,0), vec3(1,0,0), vec3(0,1,0)).inDegrees() );
		CHECK_EQUAL(90, AngleBetween(vec3(1,0,0), vec3(0,1,0), vec3(0,1,0)).inDegrees() );
		CHECK_EQUAL(180, AngleBetween(vec3(1,0,0), vec3(-1,0,0), vec3(0,1,0)).inDegrees() );
		CHECK_EQUAL(270, AngleBetween(vec3(1,0,0), vec3(0,-1,0), vec3(0,1,0)).inDegrees() );
	}

	TEST(testX)
	{
		CHECK_EQUAL(vec3(1, 0, 0), vec3(1, 0, 0));
		CHECK_EQUAL(vec3(2, 0, 0), vec3(1, 0, 0) * 2);
		CHECK_EQUAL(vec3(2, 0, 0), vec3(1, 0, 0) + vec3(1, 0, 0));
	}

	TEST(testY)
	{
		CHECK_EQUAL(vec3(0, 1, 0), vec3(0, 1, 0));
		CHECK_EQUAL(vec3(0, 2, 0), vec3(0, 1, 0) * 2);
		CHECK_EQUAL(vec3(0, 2, 0), vec3(0, 1, 0) + vec3(0, 1, 0));
	}

	TEST(testZ)
	{
		CHECK_EQUAL(vec3(0, 0, 1), vec3(0, 0, 1));
		CHECK_EQUAL(vec3(0, 0, 2), vec3(0, 0, 1) * 2);
		CHECK_EQUAL(vec3(0, 0, 2), vec3(0, 0, 1) + vec3(0, 0, 1));
	}

	TEST(testLerp)
	{
		CHECK_EQUAL(vec3(0, 1, 0), Lerp(vec3(0, 1, 0), 0, vec3(2, 4, 6)));
		CHECK_EQUAL(vec3(2, 4, 6), Lerp(vec3(0, 1, 0), 1, vec3(2, 4, 6)));
		CHECK_EQUAL(vec3(3, 5, 7), Lerp(vec3(2, 4, 6), 0.5f, vec3(4, 6, 8)));
	}
}