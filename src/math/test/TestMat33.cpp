#include "TestBase.h"

#define GTEST(x) TEST(Mat33, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	const real fe = 0.001f;

	GTEST(TestColumnMajor)
	{
		mat33 m(0,1,2,3,4,5,6,7,8);

		EXPECT_REAL_EQ(m(0, 0), 0);
		EXPECT_REAL_EQ(m(1, 0), 1);
		EXPECT_REAL_EQ(m(2, 0), 2);

		EXPECT_REAL_EQ(m(0, 1), 3);
		EXPECT_REAL_EQ(m(1, 1), 4);
		EXPECT_REAL_EQ(m(2, 1), 5);

		EXPECT_REAL_EQ(m(0, 2), 6);
		EXPECT_REAL_EQ(m(1, 2), 7);
		EXPECT_REAL_EQ(m(2, 2), 8);
	}

	GTEST(TestRowMajor)
	{
		const real temp[] = { 0,1,2,3,4,5,6,7,8 };
		mat33 m = mat33_FromRowMajor(temp);
		EXPECT_REAL_EQ(m(0, 0), 0);
		EXPECT_REAL_EQ(m(0, 1), 1);
		EXPECT_REAL_EQ(m(0, 2), 2);

		EXPECT_REAL_EQ(m(1, 0), 3);
		EXPECT_REAL_EQ(m(1, 1), 4);
		EXPECT_REAL_EQ(m(1, 2), 5);

		EXPECT_REAL_EQ(m(2, 0), 6);
		EXPECT_REAL_EQ(m(2, 1), 7);
		EXPECT_REAL_EQ(m(2, 2), 8);
	}

	GTEST(TestFromToMat44)
	{
	//	EXPECT_EQ(cmat33(cmat44(mat33Identity())), mat33Identity());
	}
}
