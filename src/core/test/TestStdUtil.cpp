#include <gtest/gtest.h>

#include <pwn/core/stdutil.h>

#define GTEST(x) TEST(StdUtil, x)

namespace test
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
		EXPECT_EQ(before-1, vec.size());
	}

	TEST_FIXTURE(fix, testSwapBackAndErase_order)
	{
		SwapBackAndEraseObject(4, &vec);
		EXPECT_EQ(1, vec[0]);
		EXPECT_EQ(3, vec[1]);
		EXPECT_EQ(2, vec[2]);
		EXPECT_EQ(-12, vec[3]);
		EXPECT_EQ(5, vec[4]);
	}

	GTEST(stringBuilderStartsEmpty)
	{
		EXPECT_EQ("", static_cast<pwn::string>(StringBuilder()));
	}

	GTEST(stringBuilderEmptyResultsInEmpty)
	{
		const pwn::string empty("");
		EXPECT_EQ(empty, static_cast<pwn::string>(StringBuilder() << empty));
	}

	GTEST(stringBuilderTextResultsInText)
	{
		const pwn::string text("abc");
		EXPECT_EQ(text, static_cast<pwn::string>(StringBuilder() << text));
	}

	GTEST(stringBuilderNumberResultsInText)
	{
		EXPECT_EQ("42", static_cast<pwn::string>(StringBuilder() << 42));
	}
}