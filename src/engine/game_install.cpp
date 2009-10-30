#include <pwn/engine/game>
#include "system_vfs.hpp"
#include "system_window.hpp"
namespace pwn
{
	namespace engine
	{
		void InstallDefaultSystems(Game* game, const pwn::string& argv0, const pwn::string& company, const pwn::string& app, const pwn::string& title)
		{
			SystemInstall_Vfs(game, argv0, company, app);
			SystemInstall_Window(game, title, 0);
		}
	}
}