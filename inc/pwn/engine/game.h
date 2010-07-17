#ifndef PWN_ENGINE_GAME
#define PWN_ENGINE_GAME

#include <pwn/string.h>
#include <memory>

namespace pwn
{
	namespace render
	{
		class World2;
	}

	namespace engine
	{
		class GameImp;
		class System;

		class Game
		{
		public:
			Game();
			~Game();

			void install(System* system); // assumes ownership
			GameImp& getImp(); // internal

			void display(int id, render::World2& world);
		private:
			std::auto_ptr<GameImp> imp;
		};

		void InstallDefaultSystems(Game* game, const pwn::string& argv0, const pwn::string& company, const pwn::string& app, const pwn::string& title);
	}
}

#endif