#include <pwn/engine/startup.h>

namespace pwn
{
	namespace engine
	{
		Startup::Startup(const pwn::string& aargv0, const pwn::string& acompany, const pwn::string& aapp, const pwn::string& atitle)
			: argv0(aargv0)
			, company(acompany)
			, app(aapp)
			, title(atitle)
		{
		}
	}
}