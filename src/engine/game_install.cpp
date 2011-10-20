#define PWN_ENGINE_NO_AUTOLINK

#include <pwn/engine/game.h>
#include "system_vfs.hpp"
#include "system_window.hpp"
#include "system_render.hpp"
#include <pwn/engine/startup.h>

namespace pwn
{
	namespace engine
	{
		void InstallDefaultSystems(Game* game, const Startup& s)
		{
			SystemInstall_Vfs(game, s.argv0, s.company, s.app);
			SystemInstall_Window(game, s.mode, s.title, 0);
			SystemInstall_Render(game);
		}
	}
}
