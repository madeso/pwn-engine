#include "TestBase.h"

#define GTEST(x) TEST(testQuat, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	GTEST(testConstants)
	{
		EXPECT_EQ(quatw(1, 0, 0, 0), qIdentity());
	}

	GTEST(testVecOp)
	{
		EXPECT_EQ(vec3(1,2,3), cvec3(quat(4, vec3(1,2,3))));
	}

	GTEST(testCommonRotation)
	{
		EXPECT_EQ(quatw(0, 1, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(180))));
		EXPECT_EQ(quatw(0, 0, 1, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(180))));
		EXPECT_EQ(quatw(0, 0, 0, 1), cquat(RightHandAround(Out(), Angle::FromDegrees(180))));

		const real k = Sqrt(0.5);
		EXPECT_EQ(quatw(k, k, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(90))));
		EXPECT_EQ(quatw(k, 0, k, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(90))));
		EXPECT_EQ(quatw(k, 0, 0, k), cquat(RightHandAround(Out(), Angle::FromDegrees(90))));

		EXPECT_EQ(quatw(k, -k, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(-90))));
		EXPECT_EQ(quatw(k, 0, -k, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(-90))));
		EXPECT_EQ(quatw(k, 0, 0, -k), cquat(RightHandAround(Out(), Angle::FromDegrees(-90))));

		EXPECT_EQ(quatw(k, k, 0, 0), cquat(RightHandAround(Left(), Angle::FromDegrees(-90))));
		EXPECT_EQ(quatw(k, 0, k, 0), cquat(RightHandAround(Down(), Angle::FromDegrees(-90))));
		EXPECT_EQ(quatw(k, 0, 0, k), cquat(RightHandAround(In(), Angle::FromDegrees(-90))));

		EXPECT_EQ(quatw(k, -k, 0, 0), cquat(RightHandAround(Left(), Angle::FromDegrees(90))));
		EXPECT_EQ(quatw(k, 0, -k, 0), cquat(RightHandAround(Down(), Angle::FromDegrees(90))));
		EXPECT_EQ(quatw(k, 0, 0, -k), cquat(RightHandAround(In(), Angle::FromDegrees(90))));
	}

	GTEST(testLocalAxis)
	{
		quat q = qIdentity();
		EXPECT_EQ(Right(), Right(q) );
		EXPECT_EQ(Up(), Up(q) );
		EXPECT_EQ(In(), In(q) );

		q = cquat(RightHandAround(Up(), Angle::FromDegrees(90)));
		EXPECT_EQ(In(), Right(q) );
		EXPECT_EQ(Up(), Up(q) );
		EXPECT_EQ(Left(), In(q) );

		q = cquat(RightHandAround(Right(), Angle::FromDegrees(90)));
		EXPECT_EQ(Right(), Right(q) );
		EXPECT_EQ(Out(), Up(q) );
		EXPECT_EQ(Up(), In(q) );

		q = cquat(RightHandAround(In(), Angle::FromDegrees(90)));
		EXPECT_EQ(Down(), Right(q) );
		EXPECT_EQ(Right(), Up(q) );
		EXPECT_EQ(In(), In(q) );
	}

	GTEST(testLook)
	{
		EXPECT_EQ(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),qLookAt(vec3(0,0,0), vec3(5,0,0), Up()) );
		EXPECT_EQ(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookAt(vec3(0,0,0), vec3(2,0,0), Up()) );
		EXPECT_EQ(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookAt(vec3(0,0,0), vec3(0.25f,0,0), Up()) );

		EXPECT_EQ(cquat(RightHandAround(Up(), Angle::FromDegrees(180))),  qLookInDirection(vec3(0,0,9), Up()) );
		EXPECT_EQ(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookInDirection(vec3(3,0,0), Up()) );
		EXPECT_EQ(cquat(RightHandAround(Up(), Angle::FromDegrees(90))),  qLookInDirection(vec3(-5,0,0), Up()) );

		// todo: add more test where up != Up()
	}

	GTEST(testCombine)
	{
		EXPECT_EQ(cquat(RightHandAround(Up(), Angle::FromDegrees(90))),
			 Combine(cquat(RightHandAround(Up(), Angle::FromDegrees(90))), qIdentity()));

		EXPECT_EQ(cquat(RightHandAround(Up(), Angle::FromDegrees(90))),
			 Combine(qIdentity(), cquat(RightHandAround(Up(), Angle::FromDegrees(90)))) );

		EXPECT_EQ( Combine(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),
			                       cquat(RightHandAround(Out(), Angle::FromDegrees(90)))),
			  Combine(cquat(RightHandAround(Right(), Angle::FromDegrees(90))),
			               cquat(RightHandAround(Up(), Angle::FromDegrees(-90)))));
	}

	GTEST(verifyTestAxisAngle)
	{
		EXPECT_EQ(RightHandAround(Up(), Angle::FromDegrees(0)), RightHandAround(Up(), Angle::FromDegrees(0)));
		EXPECT_EQ(RightHandAround(Right(), Angle::FromDegrees(90)), RightHandAround(Right(), Angle::FromDegrees(90)));
		EXPECT_EQ(RightHandAround(Up(), Angle::FromDegrees(-45)), RightHandAround(-Up(), Angle::FromDegrees(45)));
		EXPECT_EQ(RightHandAround(Right(), Angle::FromDegrees(90)), RightHandAround(-Right(), Angle::FromDegrees(-90)));
	}

	GTEST(checkAxisAngle)
	{
		EXPECT_EQ(RightHandAround(Up(), Angle::FromDegrees(0)), cAxisAngle(cquat(RightHandAround(Up(), Angle::FromDegrees(0)))));
		EXPECT_EQ(RightHandAround(Right(), Angle::FromDegrees(0)), cAxisAngle(cquat(RightHandAround(Right(), Angle::FromDegrees(0)))));
		EXPECT_EQ(RightHandAround(Right(), Angle::FromDegrees(90)), cAxisAngle(cquat(RightHandAround(Right(), Angle::FromDegrees(90)))));
		EXPECT_EQ(RightHandAround(Up(), Angle::FromDegrees(-45)), cAxisAngle(cquat(RightHandAround(-Up(), Angle::FromDegrees(45)))));
		EXPECT_EQ(RightHandAround(Right(), Angle::FromDegrees(90)), cAxisAngle(cquat(RightHandAround(-Right(), Angle::FromDegrees(-90)))));
	}

	struct Quat : ::testing::Test
	{
		quat qa;
		quat qb;

		Quat()
			: qa(cquat(RightHandAround(Up(), Angle::FromDegrees(45))))
			, qb(cquat(RightHandAround(Up(), Angle::FromDegrees(90))))
		{
		}
	};

	TEST_F(Quat, testSlerp1)
	{
		EXPECT_EQ(qIdentity(), Slerp(qIdentity(), 0, qb));
	}
	TEST_F(Quat, testSlerp2)
	{
		EXPECT_EQ(qb, Slerp(qIdentity(), 1, qb));
	}
	TEST_F(Quat, testSlerp3)
	{
		EXPECT_EQ(qIdentity(), SlerpShortway(qIdentity(), 0, qb));
	}
	TEST_F(Quat, testSlerp4)
	{
		EXPECT_EQ(qb, SlerpShortway(qIdentity(), 1, qb));
	}
	TEST_F(Quat, testSlerp5)
	{
		EXPECT_EQ(qa, SlerpShortway(qIdentity(), 0.5f, qb));
	}
}
