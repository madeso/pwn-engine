#include "TestBase.h"

#define GTEST(x) TEST(Mat33, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::math;

	const real fe = 0.001f;

	GTEST(TestColumnMajor)
	{
		const real temp[] = { 0,1,2,3,4,5,6,7,8 };
		mat33 m(temp);

		CHECK_CLOSE(m.at(0, 0), 0, fe);
		CHECK_CLOSE(m.at(1, 0), 1, fe);
		CHECK_CLOSE(m.at(2, 0), 2, fe);

		CHECK_CLOSE(m.at(0, 1), 3, fe);
		CHECK_CLOSE(m.at(1, 1), 4, fe);
		CHECK_CLOSE(m.at(2, 1), 5, fe);

		CHECK_CLOSE(m.at(0, 2), 6, fe);
		CHECK_CLOSE(m.at(1, 2), 7, fe);
		CHECK_CLOSE(m.at(2, 2), 8, fe);
	}

	GTEST(TestRowMajor)
	{
		const real temp[] = { 0,1,2,3,4,5,6,7,8 };
		mat33 m = mat33_FromRowMajor(temp);
		CHECK_CLOSE(m.at(0, 0), 0, fe);
		CHECK_CLOSE(m.at(0, 1), 1, fe);
		CHECK_CLOSE(m.at(0, 2), 2, fe);

		CHECK_CLOSE(m.at(1, 0), 3, fe);
		CHECK_CLOSE(m.at(1, 1), 4, fe);
		CHECK_CLOSE(m.at(1, 2), 5, fe);

		CHECK_CLOSE(m.at(2, 0), 6, fe);
		CHECK_CLOSE(m.at(2, 1), 7, fe);
		CHECK_CLOSE(m.at(2, 2), 8, fe);
	}

	GTEST(TestFromToMat44)
	{
		EXPECT_EQ(cmat33(cmat44(mat33Identity())), mat33Identity());
	}
}