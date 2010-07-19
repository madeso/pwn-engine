#include <gtest/gtest.h>
#include <pwn/assert.h>

int main(int argc, char **argv)
{
	pwn::assert::Init(pwn::assert::AlwaysThrow);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
