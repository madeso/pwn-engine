#include "TestBase.h"

#define GTEST(x) TEST(Angle, x)

namespace test
{
using namespace pwn;
using namespace pwn::math;

real
kTolerance()
{
    return 0.0000001f;
}

GTEST(testDeg)
{
    EXPECT_REAL_EQ(0, Angle::FromDegrees(0).inDegrees());
    EXPECT_REAL_EQ(90, Angle::FromDegrees(90).inDegrees());
    EXPECT_REAL_EQ(180, Angle::FromDegrees(180).inDegrees());
}

GTEST(testRad)
{
    EXPECT_REAL_EQ(0, Angle::FromRadians(0).inRadians());
    EXPECT_REAL_EQ(HalfPi(), Angle::FromRadians(HalfPi()).inRadians());
    EXPECT_REAL_EQ(Pi(), Angle::FromRadians(Pi()).inRadians());
}

GTEST(testAdd)
{
    EXPECT_REAL_EQ(
            90,
            (Angle::FromDegrees(90) + Angle::FromDegrees(0)).inDegrees());
    EXPECT_REAL_EQ(
            45,
            (Angle::FromDegrees(0) + Angle::FromDegrees(45)).inDegrees());
    EXPECT_REAL_EQ(
            360 + 90,
            (Angle::FromDegrees(360) + Angle::FromDegrees(90)).inDegrees());
}

GTEST(testWrap)
{
    EXPECT_REAL_EQ(90, GetWrapped(Angle::FromDegrees(90)).inDegrees());
    EXPECT_REAL_EQ(
            90,
            GetWrapped(Angle::FromDegrees(360) + Angle::FromDegrees(90))
                    .inDegrees());
}

GTEST(testSub)
{
    EXPECT_REAL_EQ(
            45,
            (Angle::FromDegrees(90) - Angle::FromDegrees(45)).inDegrees());
    EXPECT_REAL_EQ(
            90,
            (Angle::FromDegrees(90) - Angle::FromDegrees(0)).inDegrees());
}

GTEST(testConversion)
{
    EXPECT_REAL_EQ(0, Angle::FromRadians(0).inRadians());
    EXPECT_REAL_EQ(HalfPi(), Angle::FromDegrees(90).inRadians());
    EXPECT_REAL_EQ(180, Angle::FromRadians(Pi()).inDegrees());
    EXPECT_REAL_EQ(30, Angle::FromRadians(Pi() / 6).inDegrees());
    EXPECT_REAL_EQ(60, Angle::FromRadians(Pi() / 3).inDegrees());
}

real
kOneOverSqrt2()
{
    return 1.0f / Sqrt(2);
}
real
kOneOverSqrt3()
{
    return 1.0f / Sqrt(3);
}
real
kSqrt3Over2()
{
    return Sqrt(3) / 2;
}

GTEST(testSin)
{
    EXPECT_REAL_EQ(0, Sin(Angle::FromDegrees(0)));
    EXPECT_REAL_EQ(1, Sin(Angle::FromDegrees(90)));
    EXPECT_REAL_EQ(kOneOverSqrt2(), Sin(Angle::FromDegrees(45)));
    EXPECT_REAL_EQ(0.5f, Sin(Angle::FromDegrees(30)));
    EXPECT_REAL_EQ(kSqrt3Over2(), Sin(Angle::FromDegrees(60)));
}

GTEST(testCos)
{
    EXPECT_REAL_EQ(1, Cos(Angle::FromDegrees(0)));
    EXPECT_REAL_EQ(0, Cos(Angle::FromDegrees(90)));
    EXPECT_REAL_EQ(kOneOverSqrt2(), Cos(Angle::FromDegrees(45)));
    EXPECT_REAL_EQ(0.5f, Cos(Angle::FromDegrees(60)));
    EXPECT_REAL_EQ(kSqrt3Over2(), Cos(Angle::FromDegrees(30)));
}

GTEST(testTan)
{
    EXPECT_REAL_EQ(kOneOverSqrt3(), Tan(Angle::FromDegrees(30)));
}

GTEST(testASin)
{
    EXPECT_REAL_EQ(0, Asin(0).inDegrees());
    EXPECT_REAL_EQ(90, Asin(1).inDegrees());
}

GTEST(testACos)
{
    EXPECT_REAL_EQ(0, Acos(1).inDegrees());
    EXPECT_REAL_EQ(90, Acos(0).inDegrees());
}
}
