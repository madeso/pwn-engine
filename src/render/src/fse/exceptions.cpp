#include <pwn/render/fse/exceptions.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			FseException::FseException(const string& reason)
				: exception(reason.c_str())
				, message(reason)
			{
			}

			const char* FseException::what() const
			{
				return message.c_str();
			}

			void FseException::addMessage(const string& m)
			{
				message += ", " + m;
			}
		}
	}
}
