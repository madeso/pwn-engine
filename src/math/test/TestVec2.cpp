#include "TestBase.h"

SUITE(Vector2d)
{
	using namespace pwn::math;

	TEST(testLengthOf)
	{
		CHECK_EQUAL(10, LengthOf(vec2(10, 0)));
		CHECK_EQUAL(20, LengthOf(vec2(0, 20)));
		CHECK_EQUAL(0, LengthOf(vec2(0, 0)));
	}

	TEST(testArrayOp)
	{
		const vec2 ctest(7,9);
		CHECK_EQUAL(7, ctest.data()[0]);
		CHECK_EQUAL(9, ctest.data()[1]);
		CHECK_EQUAL(7, ctest[0]);
		CHECK_EQUAL(9, ctest[1]);
		CHECK_EQUAL(7, ctest.x);
		CHECK_EQUAL(9, ctest.y);

		vec2 test(4,5);
		CHECK_EQUAL(4, test.data()[0]);
		CHECK_EQUAL(5, test.data()[1]);
		CHECK_EQUAL(4, test[0]);
		CHECK_EQUAL(5, test[1]);
		CHECK_EQUAL(4, test.x);
		CHECK_EQUAL(5, test.y);
	}

	TEST(testAdd)
	{
		CHECK_EQUAL(vec2(0,1), vec2(0,1) + vec2(0,0));
		CHECK_EQUAL(vec2(0,3), vec2(0,1) + vec2(0,2));
		CHECK_EQUAL(vec2(5,0), vec2(3,0) + vec2(2,0));
		CHECK_EQUAL(vec2(3,1), vec2(3,0) + vec2(0,1));
	}

	TEST(testSub)
	{
		CHECK_EQUAL(vec2(0,1), vec2(0,2) - vec2(0,1));
		CHECK_EQUAL(vec2(0,-2), vec2(0,0) - vec2(0,2));
		CHECK_EQUAL(vec2(5,0), vec2(7,0) - vec2(2,0));
		CHECK_EQUAL(vec2(3,1), vec2(8,2) - vec2(5,1));
		CHECK_EQUAL(vec2(-5,-1), -vec2(5,1));
		CHECK_EQUAL(vec2(7,1), -vec2(-7,-1));
	}

	TEST(testDiv)
	{
		CHECK_EQUAL(vec2(4,2), vec2(8,4) / 2);
		CHECK_EQUAL(vec2(2,1), vec2(10,5) / 5);
	}

	TEST(testMult)
	{
		CHECK_EQUAL(vec2(4,2), vec2(2,1) * 2);
		CHECK_EQUAL(vec2(25,50), 5 * vec2(5,10));
	}
}