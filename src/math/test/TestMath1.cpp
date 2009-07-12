#include "TestBase.h"

SUITE(math1)
{
	using namespace pwn::math;

	TEST(testSquare)
	{
		CHECK_EQUAL(1, Square(1));
		CHECK_EQUAL(4, Square(2));
		CHECK_EQUAL(25, Square(5));
		CHECK_EQUAL(100, Square(10));
	}

	TEST(testAbs)
	{
		CHECK_EQUAL(1, Abs(1));
		CHECK_EQUAL(2, Abs(-2));
		CHECK_EQUAL(5, Abs(5));
		CHECK_EQUAL(10, Abs(-10));
	}

	TEST(testSqrt)
	{
		CHECK_EQUAL(1, Sqrt(1));
		CHECK_EQUAL(2, Sqrt(4));
		CHECK_EQUAL(5, Sqrt(25));
		CHECK_EQUAL(10, Sqrt(100));
	}

	TEST(testMin)
	{
		CHECK_EQUAL(1, Min(1, 5));
		CHECK_EQUAL(2, Min(4, 2));
		CHECK_EQUAL(-10, Min(-10, 5));
		CHECK_EQUAL(-110, Min(-10, -110));
	}

	TEST(testMax)
	{
		CHECK_EQUAL(5, Max(1, 5));
		CHECK_EQUAL(4, Max(4, 2));
		CHECK_EQUAL(5, Max(-10, 5));
		CHECK_EQUAL(-10, Max(-10, -110));
	}

	TEST(testLimmit)
	{
		CHECK_EQUAL(50, Limmit(0, 50, 100));
		CHECK_EQUAL(0, Limmit(0, -10, 100));
		CHECK_EQUAL(100, Limmit(0, 150, 100));
		CHECK_EQUAL(142, Limmit(100, 142, 156));
	}

	TEST(MathTest_MinMax)
	{
		const float max = 3;
		const float min = 2;
		CHECK_EQUAL(max, Max(max, min));
		CHECK_EQUAL(max, Max(min, max));
		CHECK_EQUAL(min, Min(min, max));
		CHECK_EQUAL(min, Min(max, min));
	}

	TEST(MathTest_360)
	{
		CHECK_EQUAL(0, Get360Angular(0, 0, 1));
		CHECK_EQUAL(0, Get360Angular(0, 0, 2));
		CHECK_EQUAL(-1, Get360Angular(-1, 0, 1));
		CHECK_EQUAL(1, Get360Angular(-1, 0.5f, 1));
		CHECK_EQUAL(1, Get360Angular(0, 0.5f, 1));
		CHECK_EQUAL(2, Get360Angular(0, 0.5f, 2));
		CHECK_EQUAL(2, Get360Angular(-1, 0.5f, 2));
	}

	TEST(MathTest_Within)
	{
		CHECK_EQUAL(true, IsWithin(0, 1, 3));
		CHECK_EQUAL(true, IsWithin(2, 2.1f, 3));
		CHECK_EQUAL(true, IsWithin(-10, -4, 0));
		CHECK_EQUAL(false, IsWithin(0, 42, 3));
		CHECK_EQUAL(false, IsWithin(0, -1, 3));
		CHECK_EQUAL(false, IsWithin(0, 3.01f, 3));

		CHECK_EQUAL(0, Within(0, -50, 100));
		CHECK_EQUAL(50, Within(0, 50, 100));
		CHECK_EQUAL(100, Within(0, 150, 100));
	}

	TEST(MathTest_Wrap)
	{
		CHECK_EQUAL(0.0f, Wrap(0.0f, 0.0f, 100.0f));
		CHECK_EQUAL(10.0f, Wrap(0.0f, 10.0f, 100.0f));
		CHECK_EQUAL(10.0f, Wrap(0.0f, 110.0f, 100.0f));
		CHECK_EQUAL(90.0f, Wrap(0.0f, -10.0f, 100.0f));

		CHECK_EQUAL(3, Wrapi(0, -1, 3));
		CHECK_EQUAL(0, Wrapi(0, 0, 3));
		CHECK_EQUAL(1, Wrapi(0, 1, 3));
		CHECK_EQUAL(2, Wrapi(0, 2, 3));
		CHECK_EQUAL(3, Wrapi(0, 3, 3));
		CHECK_EQUAL(0, Wrapi(0, 4, 3));
	}
}