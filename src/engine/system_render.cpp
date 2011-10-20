#define PWN_ENGINE_NO_AUTOLINK

#include "system_render.hpp"

#include "system.hpp"
#include <pwn/render/engine.h>
#include <pwn/engine/game.h>

namespace pwn
{
	namespace engine
	{
		class System_Render : public System
		{
		public:
			System_Render()
			{
			}

			~System_Render()
			{
			}

			void update()
			{
			}

			render::Engine engine;
		};

		void SystemInstall_Render(Game* game)
		{
			game->install(new System_Render());
		}
	}
}
