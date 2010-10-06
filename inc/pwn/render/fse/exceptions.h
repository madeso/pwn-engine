#ifndef PWN_RENDER_FSE_EXCEPTIONS
#define PWN_RENDER_FSE_EXCEPTIONS

#include <pwn/string.h>
#include <exception>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class FseException
				: public std::exception
			{
			public:
				explicit FseException(const string& what);

				virtual const char* what() const;
				void addMessage(const string& m);
			private:
				string message;
			};
		}
	}
}

#endif
