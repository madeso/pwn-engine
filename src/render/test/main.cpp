#include <UnitTest++.h>
#include <pwn/assert.h>

int main()
{
	pwn::assert::Init(pwn::assert::AlwaysThrow);
	return UnitTest::RunAllTests();
}
