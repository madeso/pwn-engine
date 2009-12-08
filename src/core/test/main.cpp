#include <UnitTest++.h>
#include <pwn/assert>

int main()
{
	pwn::assert::Init(pwn::assert::AlwaysThrow);
	return UnitTest::RunAllTests();
}
