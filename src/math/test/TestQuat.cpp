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
		CHECK_EQUAL(quatw(0, 1, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(180))));
		CHECK_EQUAL(quatw(0, 0, 1, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(180))));
		CHECK_EQUAL(quatw(0, 0, 0, 1), cquat(RightHandAround(Out(), Angle::FromDegrees(180))));

		const real k = Sqrt(0.5);
		CHECK_EQUAL(quatw(k, k, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(90))));
		CHECK_EQUAL(quatw(k, 0, k, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(90))));
		CHECK_EQUAL(quatw(k, 0, 0, k), cquat(RightHandAround(Out(), Angle::FromDegrees(90))));

		CHECK_EQUAL(quatw(k, -k, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(-90))));
		CHECK_EQUAL(quatw(k, 0, -k, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(-90))));
		CHECK_EQUAL(quatw(k, 0, 0, -k), cquat(RightHandAround(Out(), Angle::FromDegrees(-90))));

		CHECK_EQUAL(quatw(k, k, 0, 0), cquat(RightHandAround(Left(), Angle::FromDegrees(-90))));
		CHECK_EQUAL(quatw(k, 0, k, 0), cquat(RightHandAround(Down(), Angle::FromDegrees(-90))));
		CHECK_EQUAL(quatw(k, 0, 0, k), cquat(RightHandAround(In(), Angle::FromDegrees(-90))));

		CHECK_EQUAL(quatw(k, -k, 0, 0), cquat(RightHandAround(Left(), Angle::FromDegrees(90))));
		CHECK_EQUAL(quatw(k, 0, -k, 0), cquat(RightHandAround(Down(), Angle::FromDegrees(90))));
		CHECK_EQUAL(quatw(k, 0, 0, -k), cquat(RightHandAround(In(), Angle::FromDegrees(90))));
	}

	TEST(testLocalAxis)
	{
		quat q = qIdentity();
		CHECK_EQUAL(Right(), Right(q) );
		CHECK_EQUAL(Up(), Up(q) );
		CHECK_EQUAL(In(), In(q) );

		q = cquat(RightHandAround(Up(), Angle::FromDegrees(90)));
		CHECK_EQUAL(In(), Right(q) );
		CHECK_EQUAL(Up(), Up(q) );
		CHECK_EQUAL(Left(), In(q) );

		q = cquat(RightHandAround(Right(), Angle::FromDegrees(90)));
		CHECK_EQUAL(Right(), Right(q) );
		CHECK_EQUAL(Out(), Up(q) );
		CHECK_EQUAL(Up(), In(q) );

		q = cquat(RightHandAround(In(), Angle::FromDegrees(90)));
		CHECK_EQUAL(Down(), Right(q) );
		CHECK_EQUAL(Right(), Up(q) );
		CHECK_EQUAL(In(), In(q) );
	}

	TEST(testLook)
	{
		CHECK_EQUAL(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),qLookAt(vec3(0,0,0), vec3(5,0,0), Up()) );
		CHECK_EQUAL(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookAt(vec3(0,0,0), vec3(2,0,0), Up()) );
		CHECK_EQUAL(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookAt(vec3(0,0,0), vec3(0.25f,0,0), Up()) );

		CHECK_EQUAL(cquat(RightHandAround(Up(), Angle::FromDegrees(180))),  qLookInDirection(vec3(0,0,9), Up()) );
		CHECK_EQUAL(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookInDirection(vec3(3,0,0), Up()) );
		CHECK_EQUAL(cquat(RightHandAround(Up(), Angle::FromDegrees(90))),  qLookInDirection(vec3(-5,0,0), Up()) );

		// todo: add more test where up != Up()
	}

	TEST(testCombine)
	{
		CHECK_EQUAL(cquat(RightHandAround(Up(), Angle::FromDegrees(90))),
			 Combine(cquat(RightHandAround(Up(), Angle::FromDegrees(90))), qIdentity()));
		
		CHECK_EQUAL(cquat(RightHandAround(Up(), Angle::FromDegrees(90))),
			 Combine(qIdentity(), cquat(RightHandAround(Up(), Angle::FromDegrees(90)))) );

		CHECK_EQUAL( Combine(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),
			                       cquat(RightHandAround(Out(), Angle::FromDegrees(90)))),
			  Combine(cquat(RightHandAround(Right(), Angle::FromDegrees(90))),
			               cquat(RightHandAround(Up(), Angle::FromDegrees(-90)))));
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
		CHECK_EQUAL(RightHandAround(Up(), Angle::FromDegrees(0)), cAxisAngle(cquat(RightHandAround(Up(), Angle::FromDegrees(0)))));
		CHECK_EQUAL(RightHandAround(Right(), Angle::FromDegrees(0)), cAxisAngle(cquat(RightHandAround(Right(), Angle::FromDegrees(0)))));
		CHECK_EQUAL(RightHandAround(Right(), Angle::FromDegrees(90)), cAxisAngle(cquat(RightHandAround(Right(), Angle::FromDegrees(90)))));
		CHECK_EQUAL(RightHandAround(Up(), Angle::FromDegrees(-45)), cAxisAngle(cquat(RightHandAround(-Up(), Angle::FromDegrees(45)))));
		CHECK_EQUAL(RightHandAround(Right(), Angle::FromDegrees(90)), cAxisAngle(cquat(RightHandAround(-Right(), Angle::FromDegrees(-90)))));
	}
	// to axis/angle
}