#include <UnitTest++.h>

#include <pwn/core/stdutil>

SUITE(testStdUtil)
{
	using namespace pwn;
	using namespace pwn::core;

	struct fix
	{
		fix()
		{
			vec.push_back(1);
			vec.push_back(4);
			vec.push_back(2);
			vec.push_back(-12);
			vec.push_back(5);
			vec.push_back(3);
		}
		std::vector<int> vec;
	};

	TEST_FIXTURE(fix, testSwapBackAndErase_hasRemoved)
	{
		const std::size_t before = vec.size();
		SwapBackAndEraseObject(4, &vec);
		CHECK_EQUAL(before-1, vec.size());
	}

	TEST_FIXTURE(fix, testSwapBackAndErase_order)
	{
		SwapBackAndEraseObject(4, &vec);
		CHECK_EQUAL(1, vec[0]);
		CHECK_EQUAL(3, vec[1]);
		CHECK_EQUAL(2, vec[2]);
		CHECK_EQUAL(-12, vec[3]);
		CHECK_EQUAL(5, vec[4]);
	}
}