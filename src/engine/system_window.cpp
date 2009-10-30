#include "system_window.hpp"
#include "system.hpp"

#include <pwn/engine/game>
#include <SFML/Window.hpp>
#include "display.hpp"
#include "gameimp.hpp"

namespace pwn
{
	namespace engine
	{
		class System_Window : public System, Display
		{
		public:
			System_Window(Game* game, const pwn::string& title, int id)
				: imp( game->getImp() )
				, id(id)
			{
				window.Create(sf::VideoMode(800, 600, 32), title.c_str());
				imp.display_add(id, this);
			}

			~System_Window()
			{
				imp.display_remove(id, this);
			}

			void update()
			{
			}
			void render()
			{
				window.SetActive();
				window.Display();
			}
		private:
			sf::Window window;
			GameImp& imp;
			const int id;
		};

		void SystemInstall_Window(Game* game, const pwn::string& title, int id)
		{
			game->install(new System_Window(game, title, id));
		}
	}
}