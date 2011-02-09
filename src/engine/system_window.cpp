#define PWN_ENGINE_NO_AUTOLINK

#include "system_window.hpp"
#include "system.hpp"

#include <pwn/engine/game.h>
#include <pwn/engine/videomode.h>
#include <pwn/engine/key.h>
#include <pwn/assert.h>

#include <pwn/sfml-config.h>
#include <SFML/Window.hpp>
#include "display.hpp"
#include "gameimp.hpp"
#include <pwn/render/virtualdisplay.h>

#include <pwn/math/types.h>
#include <pwn/math/operations.h>

#include <pwn/sfml-config.h>
#include <SFML/OpenGL.hpp>

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
			System_Window(Game* game, const VideoMode& mode, const pwn::string& title, int id)
				: imp( game->getImp() )
				, id(id)
				, doRemove(false)
			{
				const int windowstyle = mode.fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar;
				window.Create(sf::VideoMode(mode.width, mode.height, mode.bits), title.c_str(), windowstyle);
				imp.display_add(id, this);
				doRemove = true;

				window.EnableKeyRepeat(false);
				window.ShowMouseCursor(false);
				// window.UseVerticalSync(true);

				// Indicates the quality of color, texture coordinate, and fog coordinate
				// interpolation.  If perspective-corrected parameter interpolation is not
				// efficiently supported by the GL implementation, hinting GL_DONT_CARE
				// or GL_FASTEST can result in simple linear interpolation of colors
				// and/or texture coordinates.
				Assert( glGetError() == GL_NO_ERROR);

				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
				Assert( glGetError() == GL_NO_ERROR);

				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				Assert( glGetError() == GL_NO_ERROR);

				glShadeModel(GL_SMOOTH);
				Assert( glGetError() == GL_NO_ERROR);

				glMatrixMode( GL_MODELVIEW );
				Assert( glGetError() == GL_NO_ERROR);

				glEnable(GL_CULL_FACE);
				Assert( glGetError() == GL_NO_ERROR);
			}

			~System_Window()
			{
				if( doRemove )
				{
					imp.display_remove(id, this);
				}
				window.ShowMouseCursor(true);
				window.Close();
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
						imp.handleKey(ToKey(e.MouseButton.Button), false);
						break;
					}
				}
				const sf::Input& i = window.GetInput();
				const math::vec2 size(static_cast<pwn::real>(window.GetWidth()), static_cast<pwn::real>(window.GetHeight()));
				const math::vec2 center = size / 2;
				const math::vec2 movement( (i.GetMouseX()-center.x)/ size.y, (center.y - i.GetMouseY())/size.y ); // divide by size.y (height) as movement is measured in ´percenteage of window height
				/// @todo: make it so that is determined by desktop resolution and not game size
				imp.handleMouse(movement*2);
				resetMouse();
			}
			void resetMouse()
			{
				window.SetCursorPosition(window.GetWidth()/2, window.GetHeight()/2);
			}
			void render(render::VirtualDisplay& world)
			{
				window.SetActive();
				world.render(window.GetWidth(), window.GetHeight());
				window.Display();
			}
		private:
			sf::Window window;
			GameImp& imp;
			const int id;
			bool doRemove;
		};

		void SystemInstall_Window(Game* game, const VideoMode& mode, const pwn::string& title, int id)
		{
			game->install(new System_Window(game, mode, title, id));
		}
	}
}
