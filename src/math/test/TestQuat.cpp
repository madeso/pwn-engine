#include "TestBase.h"

#define GTEST(x) TEST(testQuat, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	GTEST(testConstants)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(1, 0, 0, 0), qIdentity());
	}

	GTEST(testVecOp)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, vec3(1,2,3), cvec3(quat(4, vec3(1,2,3))));
	}

	/*GTEST(testEuclidianQuatExample)
	{
		const real k = 0.7071;
		// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/examples/index.htm
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quat(0, k, 0, k), cquat(RightHandAround(Up(), Angle::FromDegrees(90))));
	}

	GTEST(testCommonRotation)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(0, 1, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(180))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(0, 0, 1, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(180))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(0, 0, 0, 1), cquat(RightHandAround(Out(), Angle::FromDegrees(180))));

		const real k = Sqrt(0.5);
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, k, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, 0, k, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, 0, 0, k), cquat(RightHandAround(Out(), Angle::FromDegrees(90))));

		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, -k, 0, 0), cquat(RightHandAround(Right(), Angle::FromDegrees(-90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, 0, -k, 0), cquat(RightHandAround(Up(), Angle::FromDegrees(-90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, 0, 0, -k), cquat(RightHandAround(Out(), Angle::FromDegrees(-90))));

		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, k, 0, 0), cquat(RightHandAround(Left(), Angle::FromDegrees(-90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, 0, k, 0), cquat(RightHandAround(Down(), Angle::FromDegrees(-90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, 0, 0, k), cquat(RightHandAround(In(), Angle::FromDegrees(-90))));

		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, -k, 0, 0), cquat(RightHandAround(Left(), Angle::FromDegrees(90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, 0, -k, 0), cquat(RightHandAround(Down(), Angle::FromDegrees(90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, quatw(k, 0, 0, -k), cquat(RightHandAround(In(), Angle::FromDegrees(90))));
	}
	*/

	GTEST(testLocalAxis)
	{
		quat q = qIdentity();
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Right(), Right(q) );
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Up(), Up(q) );
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, In(), In(q) );

		q = cquat(RightHandAround(Up(), Angle::FromDegrees(90)));
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, In(), Right(q) );
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Up(), Up(q) );
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Left(), In(q) );

		q = cquat(RightHandAround(Right(), Angle::FromDegrees(90)));
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Right(), Right(q) );
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Out(), Up(q) );
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Up(), In(q) );

		q = cquat(RightHandAround(In(), Angle::FromDegrees(90)));
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Down(), Right(q) );
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, Right(), Up(q) );
		EXPECT_PRED_FORMAT2(::pwn::math::vec3_equal_test, In(), In(q) );
	}

	GTEST(testLook)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, cquat(RightHandAround(Up(), Angle::FromDegrees(-90))), qLookAt(vec3(0,0,0), vec3(5,0,0), Up()) );
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookAt(vec3(0,0,0), vec3(2,0,0), Up()) );
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookAt(vec3(0,0,0), vec3(0.25f,0,0), Up()) );

		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, cquat(RightHandAround(Up(), Angle::FromDegrees(180))),  qLookInDirection(vec3(0,0,-9), Up()) );
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),  qLookInDirection(vec3(3,0,0), Up()) );
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, cquat(RightHandAround(Up(), Angle::FromDegrees(90))),  qLookInDirection(vec3(-5,0,0), Up()) );

		// todo: add more test where up != Up()
	}

	GTEST(testCombine)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, cquat(RightHandAround(Up(), Angle::FromDegrees(90))),
			 Combine(cquat(RightHandAround(Up(), Angle::FromDegrees(90))), qIdentity()));

		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, cquat(RightHandAround(Up(), Angle::FromDegrees(90))),
			 Combine(qIdentity(), cquat(RightHandAround(Up(), Angle::FromDegrees(90)))) );

		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test,  Combine(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))),
			                       cquat(RightHandAround(Out(), Angle::FromDegrees(90)))),
			  Combine(cquat(RightHandAround(Right(), Angle::FromDegrees(90))),
			               cquat(RightHandAround(Up(), Angle::FromDegrees(-90)))));
	}

	GTEST(verifyTestAxisAngle)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Up(), Angle::FromDegrees(0)), RightHandAround(Up(), Angle::FromDegrees(0)));
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Right(), Angle::FromDegrees(90)), RightHandAround(Right(), Angle::FromDegrees(90)));
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Up(), Angle::FromDegrees(-45)), RightHandAround(-Up(), Angle::FromDegrees(45)));
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Right(), Angle::FromDegrees(90)), RightHandAround(-Right(), Angle::FromDegrees(-90)));
	}

	GTEST(checkAxisAngle)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Up(), Angle::FromDegrees(0)), cAxisAngle(cquat(RightHandAround(Up(), Angle::FromDegrees(0)))));
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Right(), Angle::FromDegrees(0)), cAxisAngle(cquat(RightHandAround(Right(), Angle::FromDegrees(0)))));
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Right(), Angle::FromDegrees(90)), cAxisAngle(cquat(RightHandAround(Right(), Angle::FromDegrees(90)))));
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Up(), Angle::FromDegrees(-45)), cAxisAngle(cquat(RightHandAround(-Up(), Angle::FromDegrees(45)))));
		EXPECT_PRED_FORMAT2(::pwn::math::axisangle_equal_test, RightHandAround(Right(), Angle::FromDegrees(90)), cAxisAngle(cquat(RightHandAround(-Right(), Angle::FromDegrees(-90)))));
	}

	GTEST(checkQuatConjugate)
	{
		const quat a = cquat(RightHandAround(Up(), Angle::FromDegrees(90)));
		const quat b = GetConjugate(cquat(RightHandAround(Up(), Angle::FromDegrees(-90))));
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, a, b);
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
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, qIdentity(), Slerp(qIdentity(), 0, qb));
	}
	TEST_F(Quat, testSlerp2)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, qb, Slerp(qIdentity(), 1, qb));
	}
	TEST_F(Quat, testSlerp3)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, qIdentity(), SlerpShortway(qIdentity(), 0, qb));
	}
	TEST_F(Quat, testSlerp4)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, qb, SlerpShortway(qIdentity(), 1, qb));
	}
	TEST_F(Quat, testSlerp5)
	{
		EXPECT_PRED_FORMAT2(::pwn::math::quat_equal_test, qa, SlerpShortway(qIdentity(), 0.5f, qb));
	}


}
