#include <gtest/gtest.h>

#include <pwn/core/vector.h>

#define GTEST(x) TEST(StdUtil, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::core;

	GTEST(basicConstruction)
	{
		Vector<int> vec;
	}

	GTEST(resize)
	{
		Vector<int> vec;
		vec.reset(10);
	}

	GTEST(assign)
	{
		Vector<int> vec;
		vec.reset(10);
		for(int i=0; i<10; ++i) vec[i] = i;
	}

	GTEST(swap)
	{
		Vector<int> vec;
		vec.reset(10);
		for(int i=0; i<10; ++i) vec[i] = i;

		Vector<int> v;
		v.swap(vec);
	}
}