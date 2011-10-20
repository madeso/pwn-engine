#include "TestBase.h"

#define GTEST(x) TEST(Vector2d, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	GTEST(testLengthOf)
	{
		EXPECT_REAL_EQ(10, vec2(10, 0).length());
		EXPECT_REAL_EQ(20, vec2(0, 20).length());
		EXPECT_REAL_EQ(0,  vec2(0, 0).length());
	}

	GTEST(testArrayOp)
	{
		const vec2 ctest(7, 9);
		EXPECT_REAL_EQ(7, ctest.data()[0]);
		EXPECT_REAL_EQ(9, ctest.data()[1]);
		EXPECT_REAL_EQ(7, ctest[0]);
		EXPECT_REAL_EQ(9, ctest[1]);
		EXPECT_REAL_EQ(7, X(ctest));
		EXPECT_REAL_EQ(9, Y(ctest));

		vec2 test(4, 5);
		EXPECT_REAL_EQ(4, test.data()[0]);
		EXPECT_REAL_EQ(5, test.data()[1]);
		EXPECT_REAL_EQ(4, test[0]);
		EXPECT_REAL_EQ(5, test[1]);
		EXPECT_REAL_EQ(4, X(test));
		EXPECT_REAL_EQ(5, Y(test));
	}

	GTEST(testAdd)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(0, 1), vec2(0, 1) + vec2(0, 0));
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(0, 3), vec2(0, 1) + vec2(0, 2));
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(5, 0), vec2(3, 0) + vec2(2, 0));
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(3, 1), vec2(3, 0) + vec2(0, 1));
	}

	GTEST(testSub)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(0, 1), vec2(0, 2) - vec2(0, 1));
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(0, -2), vec2(0, 0) - vec2(0, 2));
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(5, 0), vec2(7, 0) - vec2(2, 0));
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(3, 1), vec2(8, 2) - vec2(5, 1));
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(-5, -1), -vec2(5, 1));
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(7, 1), -vec2(-7, -1));
	}

	GTEST(testDiv)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(4, 2), vec2(8, 4) / 2);
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(2, 1), vec2(10, 5) / 5);
	}

	GTEST(testMult)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(4, 2), vec2(2, 1) * 2);
		EXPECT_PRED_FORMAT2(::pwn::math::vec2_equal_test, vec2(25, 50), 5 * vec2(5, 10));
	}
}
