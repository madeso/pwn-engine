#include "TestBase.h"

SUITE(testAngle)
{
	using namespace pwn;
	using namespace pwn::math;

	real kTolerance()
	{
		return 0.0000001f;
	}

	TEST(testDeg)
	{
		CHECK_EQUAL(0, Angle::FromDegrees(0).inDegrees());
		CHECK_EQUAL(90, Angle::FromDegrees(90).inDegrees());
		CHECK_EQUAL(180, Angle::FromDegrees(180).inDegrees());
	}

	TEST(testRad)
	{
		CHECK_EQUAL(0, Angle::FromRadians(0).inRadians());
		CHECK_EQUAL(HalfPi(), Angle::FromRadians(HalfPi()).inRadians());
		CHECK_EQUAL(Pi(), Angle::FromRadians(Pi()).inRadians());
	}

	TEST(testAdd)
	{
		CHECK_EQUAL(90, (Angle::FromDegrees(90) + Angle::FromDegrees(0)).inDegrees());
		CHECK_EQUAL(45, (Angle::FromDegrees(0) + Angle::FromDegrees(45)).inDegrees());
		CHECK_EQUAL(360+90, (Angle::FromDegrees(360) + Angle::FromDegrees(90)).inDegrees());
	}

	TEST(testWrap)
	{
		CHECK_EQUAL(90, GetWrapped(Angle::FromDegrees(90)).inDegrees());
		CHECK_CLOSE(90, GetWrapped(Angle::FromDegrees(360) + Angle::FromDegrees(90)).inDegrees(), 0.00001);
	}

	TEST(testSub)
	{
		CHECK_EQUAL(45, (Angle::FromDegrees(90) - Angle::FromDegrees(45)).inDegrees());
		CHECK_EQUAL(90, (Angle::FromDegrees(90) - Angle::FromDegrees(0)).inDegrees());
	}

	TEST(testConversion)
	{
		CHECK_EQUAL(0, Angle::FromRadians(0).inRadians());
		CHECK_EQUAL(HalfPi(), Angle::FromDegrees(90).inRadians());
		CHECK_EQUAL(180, Angle::FromRadians(Pi()).inDegrees());
		CHECK_EQUAL(30, Angle::FromRadians(Pi()/6).inDegrees());
		CHECK_EQUAL(60, Angle::FromRadians(Pi()/3).inDegrees());
	}

	real kOneOverSqrt2()
	{
		return 1.0f / Sqrt(2);
	}
	real kOneOverSqrt3()
	{
		return 1.0f / Sqrt(3);
	}
	real kSqrt3Over2()
	{
		return Sqrt(3)/2;
	}

	TEST(testSin)
	{
		CHECK_CLOSE(0, Sin(Angle::FromDegrees(0)), kTolerance());
		CHECK_CLOSE(1, Sin(Angle::FromDegrees(90)), kTolerance());
		CHECK_CLOSE(kOneOverSqrt2(), Sin(Angle::FromDegrees(45)), kTolerance());
		CHECK_CLOSE(0.5f, Sin(Angle::FromDegrees(30)), kTolerance());
		CHECK_CLOSE(kSqrt3Over2(), Sin(Angle::FromDegrees(60)), kTolerance());
	}

	TEST(testCos)
	{
		CHECK_CLOSE(1, Cos(Angle::FromDegrees(0)), kTolerance());
		CHECK_CLOSE(0, Cos(Angle::FromDegrees(90)), kTolerance());
		CHECK_CLOSE(kOneOverSqrt2(), Cos(Angle::FromDegrees(45)), kTolerance());
		CHECK_CLOSE(0.5f, Cos(Angle::FromDegrees(60)), kTolerance());
		CHECK_CLOSE(kSqrt3Over2(), Cos(Angle::FromDegrees(30)), kTolerance());
	}

	TEST(testTan)
	{
		CHECK_CLOSE(kOneOverSqrt3(), Tan(Angle::FromDegrees(30)), kTolerance());
	}

	TEST(testASin)
	{
		CHECK_CLOSE(0, Asin(0).inDegrees(), kTolerance());
		CHECK_CLOSE(90, Asin(1).inDegrees(), kTolerance());
	}

	TEST(testACos)
	{
		CHECK_CLOSE(0, Acos(1).inDegrees(), kTolerance());
		CHECK_CLOSE(90, Acos(0).inDegrees(), kTolerance());
	}
}