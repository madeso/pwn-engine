#include <gtest/gtest.h>

#include <pwn/core/idpool.h>

#define GTEST(x) TEST(StdUtil, x)

namespace test
{
using namespace pwn;
using namespace pwn::core;

GTEST(idpoolCreateTest)
{
    IdPool pool;
    IdPool::ID first = pool.generate();
    IdPool::ID second = pool.generate();
    EXPECT_NE(first, second);
}

GTEST(idpoolCreateIdObjectTest)
{
    IdPool pool;
    Id first(&pool);
    Id second(&pool);
    EXPECT_NE(first.value, second.value);
}

GTEST(idpoolReleaseTest)
{
    IdPool pool;
    IdPool::ID first = pool.generate();
    pool.release(first);
    IdPool::ID second = pool.generate();
    EXPECT_EQ(first, second);
}

GTEST(idpoolIdObjectTest)
{
    IdPool pool;
    IdPool::ID first;
    {
        Id useless(&pool);
        first = useless.value;
    }
    Id second(&pool);
    EXPECT_EQ(first, second.value);
}
}
