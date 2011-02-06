#include "TestBase.h"

#define GTEST(x) TEST(Vector2d, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	GTEST(testLengthOf)
	{
		EXPECT_EQ(10, LengthOf(vec2(10, 0)));
		EXPECT_EQ(20, LengthOf(vec2(0, 20)));
		EXPECT_EQ(0, LengthOf(vec2(0, 0)));
	}

	GTEST(testArrayOp)
	{
		const vec2 ctest(7,9);
		EXPECT_EQ(7, ctest.data()[0]);
		EXPECT_EQ(9, ctest.data()[1]);
		EXPECT_EQ(7, ctest[0]);
		EXPECT_EQ(9, ctest[1]);
		EXPECT_EQ(7, ctest.x);
		EXPECT_EQ(9, ctest.y);

		vec2 test(4,5);
		EXPECT_EQ(4, test.data()[0]);
		EXPECT_EQ(5, test.data()[1]);
		EXPECT_EQ(4, test[0]);
		EXPECT_EQ(5, test[1]);
		EXPECT_EQ(4, test.x);
		EXPECT_EQ(5, test.y);
	}

	GTEST(testAdd)
	{
		EXPECT_EQ(vec2(0,1), vec2(0,1) + vec2(0,0));
		EXPECT_EQ(vec2(0,3), vec2(0,1) + vec2(0,2));
		EXPECT_EQ(vec2(5,0), vec2(3,0) + vec2(2,0));
		EXPECT_EQ(vec2(3,1), vec2(3,0) + vec2(0,1));
	}

	GTEST(testSub)
	{
		EXPECT_EQ(vec2(0,1), vec2(0,2) - vec2(0,1));
		EXPECT_EQ(vec2(0,-2), vec2(0,0) - vec2(0,2));
		EXPECT_EQ(vec2(5,0), vec2(7,0) - vec2(2,0));
		EXPECT_EQ(vec2(3,1), vec2(8,2) - vec2(5,1));
		EXPECT_EQ(vec2(-5,-1), -vec2(5,1));
		EXPECT_EQ(vec2(7,1), -vec2(-7,-1));
	}

	GTEST(testDiv)
	{
		EXPECT_EQ(vec2(4,2), vec2(8,4) / 2);
		EXPECT_EQ(vec2(2,1), vec2(10,5) / 5);
	}

	GTEST(testMult)
	{
		EXPECT_EQ(vec2(4,2), vec2(2,1) * 2);
		EXPECT_EQ(vec2(25,50), 5 * vec2(5,10));
	}
}
