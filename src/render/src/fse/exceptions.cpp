#include <pwn/render/fse/exceptions.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			FseException::FseException(const string& reason)
				:
				// apperently gcc on my ubuntu doesnt support std::exception(string-description)
#ifdef _MSVC_VER
				exception(reason.c_str()),
#endif
				message(reason)
			{
			}

			FseException::~FseException() throw()
			{
			}

			const char* FseException::what() const throw()
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
