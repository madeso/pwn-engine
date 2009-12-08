#ifndef PWN_ASSERT
#define PWN_ASSERT

#include <string>

#define Assert(x) do { if( (x)==0 && ::pwn::assert::ShouldBreak(#x) ) { _asm { int 3 }; } } while( ::pwn::assert::kFalse() )

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