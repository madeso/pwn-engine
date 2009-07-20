#include "TestBase.h"

SUITE(testQuat)
{
	using namespace pwn::math;

	TEST(testConstants)
	{
		CHECK_EQUAL(quatw(1, 0, 0, 0), qIdentity());
	}

	TEST(testCommonRotation)
	{
		CHECK_EQUAL(quatw(0, 1, 0, 0), quat(RightHandAround(Right(), Angle::FromDegrees(180))));
		CHECK_EQUAL(quatw(0, 0, 1, 0), quat(RightHandAround(Up(), Angle::FromDegrees(180))));
		CHECK_EQUAL(quatw(0, 0, 0, 1), quat(RightHandAround(Out(), Angle::FromDegrees(180))));

		const real k = Sqrt(0.5);
		CHECK_EQUAL(quatw(k, k, 0, 0), quat(RightHandAround(Right(), Angle::FromDegrees(90))));
		CHECK_EQUAL(quatw(k, 0, k, 0), quat(RightHandAround(Up(), Angle::FromDegrees(90))));
		CHECK_EQUAL(quatw(k, 0, 0, k), quat(RightHandAround(Out(), Angle::FromDegrees(90))));

		CHECK_EQUAL(quatw(k, -k, 0, 0), quat(RightHandAround(Right(), Angle::FromDegrees(-90))));
		CHECK_EQUAL(quatw(k, 0, -k, 0), quat(RightHandAround(Up(), Angle::FromDegrees(-90))));
		CHECK_EQUAL(quatw(k, 0, 0, -k), quat(RightHandAround(Out(), Angle::FromDegrees(-90))));

		CHECK_EQUAL(quatw(k, k, 0, 0), quat(RightHandAround(Left(), Angle::FromDegrees(-90))));
		CHECK_EQUAL(quatw(k, 0, k, 0), quat(RightHandAround(Down(), Angle::FromDegrees(-90))));
		CHECK_EQUAL(quatw(k, 0, 0, k), quat(RightHandAround(In(), Angle::FromDegrees(-90))));

		CHECK_EQUAL(quatw(k, -k, 0, 0), quat(RightHandAround(Left(), Angle::FromDegrees(90))));
		CHECK_EQUAL(quatw(k, 0, -k, 0), quat(RightHandAround(Down(), Angle::FromDegrees(90))));
		CHECK_EQUAL(quatw(k, 0, 0, -k), quat(RightHandAround(In(), Angle::FromDegrees(90))));
	}

	TEST(testLocalAxis)
	{
		quat q = qIdentity();
		CHECK_EQUAL(Right(), Right(q) );
		CHECK_EQUAL(Up(), Up(q) );
		CHECK_EQUAL(In(), In(q) );

		q = quat(RightHandAround(Up(), Angle::FromDegrees(90)));
		CHECK_EQUAL(In(), Right(q) );
		CHECK_EQUAL(Up(), Up(q) );
		CHECK_EQUAL(Left(), In(q) );

		q = quat(RightHandAround(Right(), Angle::FromDegrees(90)));
		CHECK_EQUAL(Right(), Right(q) );
		CHECK_EQUAL(Out(), Up(q) );
		CHECK_EQUAL(Up(), In(q) );

		q = quat(RightHandAround(In(), Angle::FromDegrees(90)));
		CHECK_EQUAL(Down(), Right(q) );
		CHECK_EQUAL(Right(), Up(q) );
		CHECK_EQUAL(In(), In(q) );
	}

	TEST(testLook)
	{
		CHECK_EQUAL(quat(RightHandAround(Up(), Angle::FromDegrees(-90))),qLookAt(vec3(0,0,0), vec3(5,0,0), Up()) );
		CHECK_EQUAL(quat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookAt(vec3(0,0,0), vec3(2,0,0), Up()) );
		CHECK_EQUAL(quat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookAt(vec3(0,0,0), vec3(0.25f,0,0), Up()) );

		CHECK_EQUAL(quat(RightHandAround(Up(), Angle::FromDegrees(180))),  qLookInDirection(vec3(0,0,9), Up()) );
		CHECK_EQUAL(quat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookInDirection(vec3(3,0,0), Up()) );
		CHECK_EQUAL(quat(RightHandAround(Up(), Angle::FromDegrees(90))),  qLookInDirection(vec3(-5,0,0), Up()) );

		// todo: add more test where up != Up()
	}

	TEST(testCombine)
	{
		CHECK_EQUAL(quat(RightHandAround(Up(), Angle::FromDegrees(90))),
			 Combine(quat(RightHandAround(Up(), Angle::FromDegrees(90))), qIdentity()));
		
		CHECK_EQUAL(quat(RightHandAround(Up(), Angle::FromDegrees(90))),
			 Combine(qIdentity(), quat(RightHandAround(Up(), Angle::FromDegrees(90)))) );

		CHECK_EQUAL( Combine(quat(RightHandAround(Up(), Angle::FromDegrees(-90))),
			                       quat(RightHandAround(Out(), Angle::FromDegrees(90)))),
			  Combine(quat(RightHandAround(Right(), Angle::FromDegrees(90))),
			               quat(RightHandAround(Up(), Angle::FromDegrees(-90)))));
	}

	TEST(verifyTestAxisAngle)
	{
		CHECK_EQUAL(RightHandAround(Up(), Angle::FromDegrees(0)), RightHandAround(Up(), Angle::FromDegrees(0)));
		CHECK_EQUAL(RightHandAround(Right(), Angle::FromDegrees(90)), RightHandAround(Right(), Angle::FromDegrees(90)));
		CHECK_EQUAL(RightHandAround(Up(), Angle::FromDegrees(-45)), RightHandAround(-Up(), Angle::FromDegrees(45)));
		CHECK_EQUAL(RightHandAround(Right(), Angle::FromDegrees(90)), RightHandAround(-Right(), Angle::FromDegrees(-90)));
	}

	TEST(checkAxisAngle)
	{
		CHECK_EQUAL(RightHandAround(Up(), Angle::FromDegrees(0)), AxisAngle(quat(RightHandAround(Up(), Angle::FromDegrees(0)))));
		CHECK_EQUAL(RightHandAround(Right(), Angle::FromDegrees(0)), AxisAngle(quat(RightHandAround(Right(), Angle::FromDegrees(0)))));
		CHECK_EQUAL(RightHandAround(Right(), Angle::FromDegrees(90)), AxisAngle(quat(RightHandAround(Right(), Angle::FromDegrees(90)))));
		CHECK_EQUAL(RightHandAround(Up(), Angle::FromDegrees(-45)), AxisAngle(quat(RightHandAround(-Up(), Angle::FromDegrees(45)))));
		CHECK_EQUAL(RightHandAround(Right(), Angle::FromDegrees(90)), AxisAngle(quat(RightHandAround(-Right(), Angle::FromDegrees(-90)))));
	}
	// to axis/angle
}