#ifndef PWN_CORE_STR
#define PWN_CORE_STR

#include <pwn/string.h>

namespace pwn
{
	namespace core
	{
		class Str
		{
		public:
			operator pwn::string() const
			{
				return toString();
			}

			pwn::string toString() const
			{
				return stream.str();
			}

			template<typename Type>
			Str& operator<<(const Type& t)
			{
				stream << t;
				return *this;
			}
		private:
			pwn::ostringstream stream;
		};
	}
}

#endif
