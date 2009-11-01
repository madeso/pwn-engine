#define PWN_ENGINE_NO_AUTOLINK

#include <pwn/engine/game>
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