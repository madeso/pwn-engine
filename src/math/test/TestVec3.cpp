#include "TestBase.h"

#define GTEST(x) TEST(Vector3d, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	GTEST(testLengthOf)
	{
		EXPECT_EQ(10, LengthOf(vec3(10, 0, 0)));
		EXPECT_EQ(20, LengthOf(vec3(0, 20, 0)));
		EXPECT_EQ(30, LengthOf(vec3(0, 0, 30)));
		EXPECT_EQ(0, LengthOf(vec3(0, 0, 0)));
	}

	GTEST(testArrayOp)
	{
		const vec3 ctest(7,9, 4);
		EXPECT_EQ(7, ctest.data()[0]);
		EXPECT_EQ(9, ctest.data()[1]);
		EXPECT_EQ(4, ctest.data()[2]);
		EXPECT_EQ(7, ctest[0]);
		EXPECT_EQ(9, ctest[1]);
		EXPECT_EQ(4, ctest[2]);
		EXPECT_EQ(7, ctest.x);
		EXPECT_EQ(9, ctest.y);
		EXPECT_EQ(4, ctest.z);

		vec3 test(4,5, 6);
		EXPECT_EQ(4, test.data()[0]);
		EXPECT_EQ(5, test.data()[1]);
		EXPECT_EQ(6, test.data()[2]);
		EXPECT_EQ(4, test[0]);
		EXPECT_EQ(5, test[1]);
		EXPECT_EQ(6, test[2]);
		EXPECT_EQ(4, test.x);
		EXPECT_EQ(5, test.y);
		EXPECT_EQ(6, test.z);
	}

	GTEST(testAdd)
	{
		EXPECT_EQ(vec3(4,4,4), vec3(1,2,3) + vec3(3,2,1));
		EXPECT_EQ(vec3(1,2,3), vec3(-1,1,4) + vec3(2,1,-1));
	}

	GTEST(testSub)
	{
		EXPECT_EQ(vec3(0,1,0), vec3(0,2,0) - vec3(0,1,0));
		EXPECT_EQ(vec3(-2,0,2), vec3(1,2,3) - vec3(3,2,1));
	}

	GTEST(testDiv)
	{
		EXPECT_EQ(vec3(4,2,1), vec3(8,4,2) / 2);
		EXPECT_EQ(vec3(5,2,1), vec3(25,10,5) / 5);
	}

	GTEST(testMult)
	{
		EXPECT_EQ(vec3(8,4,2), vec3(4,2,1) * 2);
		EXPECT_EQ(vec3(5,25,50), 5 * vec3(1,5,10));
	}

	GTEST(testNormal)
	{
		EXPECT_EQ(vec3(1,0,0), GetNormalized(vec3(4,0,0) ));
		EXPECT_EQ(vec3(0,1,0), GetNormalized(vec3(0,9,0) ));
	}

	GTEST(testDot)
	{
		EXPECT_EQ(0, dot(vec3(1,0,0), vec3(0,1,0) ));
		EXPECT_EQ(1, dot(vec3(1,0,0), vec3(1,0,0) ));
	}

	GTEST(testCross)
	{
		EXPECT_EQ(vec3(0,1,0), cross(vec3(0,0,-1), vec3(-1,0,0) ));
	}

	GTEST(testAngle)
	{
		// Angle between 2, 0-180
		EXPECT_EQ(0, AngleBetween(vec3(1,0,0), vec3(1,0,0)).inDegrees() );
		EXPECT_EQ(90, AngleBetween(vec3(1,0,0), vec3(0,1,0)).inDegrees() );
		EXPECT_EQ(180, AngleBetween(vec3(1,0,0), vec3(-1,0,0)).inDegrees() );
		EXPECT_EQ(90, AngleBetween(vec3(1,0,0), vec3(0,-1,0)).inDegrees() );

		// specify extra - 0-360
		EXPECT_EQ(0, AngleBetween(vec3(1,0,0), vec3(1,0,0), vec3(0,1,0)).inDegrees() );
		EXPECT_EQ(90, AngleBetween(vec3(1,0,0), vec3(0,1,0), vec3(0,1,0)).inDegrees() );
		EXPECT_EQ(180, AngleBetween(vec3(1,0,0), vec3(-1,0,0), vec3(0,1,0)).inDegrees() );
		EXPECT_EQ(270, AngleBetween(vec3(1,0,0), vec3(0,-1,0), vec3(0,1,0)).inDegrees() );
	}

	GTEST(testX)
	{
		EXPECT_EQ(vec3(1, 0, 0), vec3(1, 0, 0));
		EXPECT_EQ(vec3(2, 0, 0), vec3(1, 0, 0) * 2);
		EXPECT_EQ(vec3(2, 0, 0), vec3(1, 0, 0) + vec3(1, 0, 0));
	}

	GTEST(testY)
	{
		EXPECT_EQ(vec3(0, 1, 0), vec3(0, 1, 0));
		EXPECT_EQ(vec3(0, 2, 0), vec3(0, 1, 0) * 2);
		EXPECT_EQ(vec3(0, 2, 0), vec3(0, 1, 0) + vec3(0, 1, 0));
	}

	GTEST(testZ)
	{
		EXPECT_EQ(vec3(0, 0, 1), vec3(0, 0, 1));
		EXPECT_EQ(vec3(0, 0, 2), vec3(0, 0, 1) * 2);
		EXPECT_EQ(vec3(0, 0, 2), vec3(0, 0, 1) + vec3(0, 0, 1));
	}

	GTEST(testLerp)
	{
		EXPECT_EQ(vec3(0, 1, 0), Lerp(vec3(0, 1, 0), 0, vec3(2, 4, 6)));
		EXPECT_EQ(vec3(2, 4, 6), Lerp(vec3(0, 1, 0), 1, vec3(2, 4, 6)));
		EXPECT_EQ(vec3(3, 5, 7), Lerp(vec3(2, 4, 6), 0.5f, vec3(4, 6, 8)));
	}
}