#define PWN_ENGINE_NO_AUTOLINK

#include "system_window.hpp"
#include "system.hpp"

#include <pwn/engine/game>
#include <pwn/engine/key>

#include <SFML/Window.hpp>
#include "display.hpp"
#include "gameimp.hpp"
#include <pwn/render/world2>

#include <pwn/math/types>
#include <pwn/math/operations>

#include <SFML/OpenGl.hpp>

#ifdef _DEBUG
#define LIBRARY_SUFFIX "-d"
#else
#define LIBRARY_SUFFIX ""
#endif

#pragma comment(lib, "sfml-main" LIBRARY_SUFFIX ".lib" )
#pragma comment(lib, "sfml-system" LIBRARY_SUFFIX ".lib" )
#pragma comment(lib, "sfml-window" LIBRARY_SUFFIX ".lib" )

namespace pwn
{
	namespace engine
	{
		namespace
		{
			pwn::engine::Key::Code ToKey(sf::Key::Code k)
			{
				return static_cast<pwn::engine::Key::Code>(k);
			}
			pwn::engine::Key::Code ToKey(sf::Mouse::Button k)
			{
				switch(k)
				{
				case sf::Mouse::Left: return Key::LMB;
				case sf::Mouse::Right: return Key::RMB;
				case sf::Mouse::Middle: return Key::MMB;
				case sf::Mouse::XButton1: return Key::X1;
				case sf::Mouse::XButton2: return Key::X2;
				default: return Key::Undefined;
				}
			}
		}

		class System_Window : public System, Display
		{
		public:
			System_Window(Game* game, const pwn::string& title, int id)
				: imp( game->getImp() )
				, id(id)
				, doRemove(false)
			{
				window.Create(sf::VideoMode(800, 600, 32), title.c_str());
				imp.display_add(id, this);
				doRemove = true;

				window.EnableKeyRepeat(false);
				// window.UseVerticalSync(true);

				// Indicates the quality of color, texture coordinate, and fog coordinate
				// interpolation.  If perspective-corrected parameter interpolation is not
				// efficiently supported by the GL implementation, hinting GL_DONT_CARE
				// or GL_FASTEST can result in simple linear interpolation of colors
				// and/or texture coordinates.
				assert( glGetError() == GL_NO_ERROR);
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
				assert( glGetError() == GL_NO_ERROR);
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				assert( glGetError() == GL_NO_ERROR);
				glShadeModel(GL_SMOOTH);
				assert( glGetError() == GL_NO_ERROR);

				glMatrixMode( GL_MODELVIEW );
				assert( glGetError() == GL_NO_ERROR);

				glEnable(GL_CULL_FACE);
				assert( glGetError() == GL_NO_ERROR);
			}

			~System_Window()
			{
				if( doRemove )
				{
					imp.display_remove(id, this);
				}
			}

			void update()
			{
				sf::Event e;
				while (window.GetEvent(e))
				{
					switch(e.Type)
					{
					case sf::Event::Closed:
						break;
					case sf::Event::KeyPressed:
						imp.handleKey(ToKey(e.Key.Code), true);
						break;
					case sf::Event::KeyReleased:
						imp.handleKey(ToKey(e.Key.Code), false);
						break;
					case sf::Event::MouseButtonPressed:
						imp.handleKey(ToKey(e.MouseButton.Button), true);
						break;
					case sf::Event::MouseButtonReleased:
						imp.handleKey(ToKey(e.MouseButton.Button), true);
						break;
					}
				}
				const sf::Input& i = window.GetInput();
				const math::vec2 size(window.GetWidth(), window.GetHeight());
				const math::vec2 center = size / 2;
				const math::vec2 movement( (i.GetMouseX()-center.x)/ size.x, (center.y - i.GetMouseY())/size.y );
				imp.handleMouse(movement);
				resetMouse();
			}
			void resetMouse()
			{
				window.SetCursorPosition(window.GetWidth()/2, window.GetHeight()/2);
			}
			void render(render::World2& world)
			{
				window.SetActive();
				world.render();
				window.Display();
			}
		private:
			sf::Window window;
			GameImp& imp;
			const int id;
			bool doRemove;
		};

		void SystemInstall_Window(Game* game, const pwn::string& title, int id)
		{
			game->install(new System_Window(game, title, id));
		}
	}
}