#include "TestBase.h"

#define GTEST(x) TEST(Math1, x)

namespace test
{
using namespace pwn;
using namespace pwn::math;

GTEST(testSquare)
{
    EXPECT_EQ(1, Square(1));
    EXPECT_EQ(4, Square(2));
    EXPECT_EQ(25, Square(5));
    EXPECT_EQ(100, Square(10));
}

GTEST(testAbs)
{
    EXPECT_EQ(1, Abs(1));
    EXPECT_EQ(2, Abs(-2));
    EXPECT_EQ(5, Abs(5));
    EXPECT_EQ(10, Abs(-10));
}

GTEST(testSqrt)
{
    EXPECT_EQ(1, Sqrt(1));
    EXPECT_EQ(2, Sqrt(4));
    EXPECT_EQ(5, Sqrt(25));
    EXPECT_EQ(10, Sqrt(100));
}

GTEST(testMin)
{
    EXPECT_EQ(1, Min(1, 5));
    EXPECT_EQ(2, Min(4, 2));
    EXPECT_EQ(-10, Min(-10, 5));
    EXPECT_EQ(-110, Min(-10, -110));
}

GTEST(testMax)
{
    EXPECT_EQ(5, Max(1, 5));
    EXPECT_EQ(4, Max(4, 2));
    EXPECT_EQ(5, Max(-10, 5));
    EXPECT_EQ(-10, Max(-10, -110));
}

GTEST(MathTest_MinMax)
{
    const float max = 3;
    const float min = 2;
    EXPECT_EQ(max, Max(max, min));
    EXPECT_EQ(max, Max(min, max));
    EXPECT_EQ(min, Min(min, max));
    EXPECT_EQ(min, Min(max, min));
}

GTEST(MathTest_360)
{
    EXPECT_EQ(0, Get360Angular(0, 0, 1));
    EXPECT_EQ(0, Get360Angular(0, 0, 2));
    EXPECT_EQ(-1, Get360Angular(-1, 0, 1));
    EXPECT_EQ(1, Get360Angular(-1, 0.5f, 1));
    EXPECT_EQ(1, Get360Angular(0, 0.5f, 1));
    EXPECT_EQ(2, Get360Angular(0, 0.5f, 2));
    EXPECT_EQ(2, Get360Angular(-1, 0.5f, 2));
}

GTEST(testWithin)
{
    EXPECT_EQ(50, KeepWithin(0, 50, 100));
    EXPECT_EQ(0, KeepWithin(0, -10, 100));
    EXPECT_EQ(100, KeepWithin(0, 150, 100));
    EXPECT_EQ(142, KeepWithin(100, 142, 156));
}

GTEST(MathTest_IsWithin)
{
    EXPECT_EQ(true, IsWithin(0, 1, 3));
    EXPECT_EQ(true, IsWithin(2, 2.1f, 3));
    EXPECT_EQ(true, IsWithin(-10, -4, 0));
    EXPECT_EQ(false, IsWithin(0, 42, 3));
    EXPECT_EQ(false, IsWithin(0, -1, 3));
    EXPECT_EQ(false, IsWithin(0, 3.01f, 3));
}

GTEST(MathTest_Wrap)
{
    EXPECT_EQ(0.0f, Wrap(0.0f, 0.0f, 100.0f));
    EXPECT_EQ(10.0f, Wrap(0.0f, 10.0f, 100.0f));
    EXPECT_EQ(10.0f, Wrap(0.0f, 110.0f, 100.0f));
    EXPECT_EQ(90.0f, Wrap(0.0f, -10.0f, 100.0f));

    EXPECT_EQ(3, Wrapi(0, -1, 3));
    EXPECT_EQ(0, Wrapi(0, 0, 3));
    EXPECT_EQ(1, Wrapi(0, 1, 3));
    EXPECT_EQ(2, Wrapi(0, 2, 3));
    EXPECT_EQ(3, Wrapi(0, 3, 3));
    EXPECT_EQ(0, Wrapi(0, 4, 3));
}
}
