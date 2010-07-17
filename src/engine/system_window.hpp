#ifndef PWN_ENGINE_SYSTEM_WINDOW_HPP
#define PWN_ENGINE_SYSTEM_WINDOW_HPP

#include <pwn/string.h>

namespace pwn
{
	namespace engine
	{
		class Game;
		void SystemInstall_Window(Game* game, const pwn::string& title, int id);
	}
}

#endif