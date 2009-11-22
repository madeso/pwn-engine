#include <UnitTest++.h>

#include <pwn/core/idpool>

SUITE(testStdUtil)
{
	using namespace pwn;
	using namespace pwn::core;

	TEST(idpoolCreateTest)
	{
		IdPool pool;
		IdPool::ID first = pool.generate();
		IdPool::ID second = pool.generate();
		CHECK(first != second );
	}

	TEST(idpoolReleaseTest)
	{
		IdPool pool;
		IdPool::ID first = pool.generate();
		pool.release(first);
		IdPool::ID second = pool.generate();
		CHECK_EQUAL(first, second );
	}
}