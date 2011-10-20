#include <pwn/assert.h>

namespace pwn
{
	namespace assert
	{

		namespace //local
		{
			AssertAction& AA()
			{
				static AssertAction aa = AlwaysBreak;
				return aa;
			}
		}

		bool kFalse()
		{
			return false;
		}

		bool ShouldBreak(const std::string& expression)
		{
			switch(AA())
			{
				case AlwaysBreak:
					return true;
				case AlwaysThrow:
					throw expression;
				default:
					throw "invalid assert state, bug?";
			}
		}

		void Init(AssertAction aa)
		{
			AA() = aa;
		}
	}
}
