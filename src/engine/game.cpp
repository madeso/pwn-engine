#define PWN_ENGINE_NO_AUTOLINK

#include <pwn/engine/game.h>
#include "gameimp.hpp"

namespace pwn
{
	namespace engine
	{
		Game::Game()
			: imp( new GameImp() )
		{
		}

		Game::~Game()
		{
			imp.reset();
		}

		void Game::install(System* system)
		{
			imp->install(system);
		}

		GameImp& Game::getImp()
		{
			return *imp;
		}
	}
}