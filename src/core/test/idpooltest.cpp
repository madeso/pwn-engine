#include <UnitTest++.h>

#include <pwn/core/idpool.h>

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

	TEST(idpoolCreateIdObjectTest)
	{
		IdPool pool;
		Id first(&pool);
		Id second(&pool);
		CHECK(first.value != second.value );
	}

	TEST(idpoolReleaseTest)
	{
		IdPool pool;
		IdPool::ID first = pool.generate();
		pool.release(first);
		IdPool::ID second = pool.generate();
		CHECK_EQUAL(first, second );
	}

	TEST(idpoolIdObjectTest)
	{
		IdPool pool;
		IdPool::ID first;
		{
			Id useless(&pool);
			first = useless.value;
		}
		Id second(&pool);
		CHECK_EQUAL(first, second.value );
	}
}