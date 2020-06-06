#ifndef PWN_ENGINE_STARTUP
#define PWN_ENGINE_STARTUP

#include <pwn/string.h>
#include <pwn/engine/videomode.h>

namespace pwn
{
	namespace engine
	{
		/** Help class for starting up a game.
		*/
		class Startup
		{
		public:
			Startup(const pwn::string& argv0, const pwn::string& company, const pwn::string& app, const pwn::string& title);

			pwn::string argv0;
			pwn::string company;
			pwn::string app;
			pwn::string title;

			VideoMode mode;
		};
	}
}

#endif
