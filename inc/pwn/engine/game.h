#ifndef PWN_ENGINE_GAME
#define PWN_ENGINE_GAME

#include <pwn/string.h>
#include <memory>

namespace pwn
{
	namespace render
	{
		class VirtualDisplay;
	}

	namespace engine
	{
		class GameImp;
		class System;
		class Startup;

		class Game
		{
		public:
			Game();
			~Game();

			void install(System* system); // assumes ownership
			GameImp& getImp(); // internal

			void display(int id, render::VirtualDisplay& world);
		private:
			std::auto_ptr<GameImp> imp;
		};

		void InstallDefaultSystems(Game* game, const Startup& startup);
	}
}

#endif
