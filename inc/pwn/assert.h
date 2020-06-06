#ifndef PWN_ASSERT
#define PWN_ASSERT

#include <string>

#ifdef _MSC_VER
#define pwnBreakpoint() __debugbreak()
#else
// todo: come up with a better solution for asserting
#include <cassert>
#define pwnBreakpoint() assert(false)
#endif

#define Assert(x) do { if( (x)==0 && ::pwn::assert::ShouldBreak(#x) ) { pwnBreakpoint(); } } while( ::pwn::assert::kFalse() )

namespace pwn
{
	namespace assert
	{
		enum AssertAction
		{
		   AlwaysBreak
		   , AlwaysThrow
		};

		bool kFalse();
		bool ShouldBreak(const std::string& expression);
		void Init(AssertAction aa);
	}
}

#endif
