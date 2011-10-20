#define PWN_ENGINE_NO_AUTOLINK

#include "gameimp.hpp"
#include "system.hpp"
#include <algorithm>
#include "loop.hpp"
#include <pwn/engine/loop.h>
#include "display.hpp"

namespace pwn
{
	namespace engine
	{
		GameImp::GameImp()
		{
		}

		GameImp::~GameImp()
		{
			// important! all systems must be destroyed and destructors need to be run before std clears the display refered in the destructors...
			systems.clear();
		}

		void GameImp::install(System* system)
		{
			SystemPtr s(system);
			systems.push_back(s);
		}

		namespace // local
		{
			void UpdateSystem(GameImp::SystemPtr& system)
			{
				system->update();
			}
		}

		void GameImp::updateSystems()
		{
			std::for_each(systems.begin(), systems.end(), UpdateSystem);
		}

		void GameImp::display_add(int id, Display* disp)
		{
			const DisplayMap::iterator res = displays.find(id);
			if(res != displays.end() && res->second != 0)
			{
				throw "id belongs to someone else";
			}
			displays[id] = disp;
		}

		void GameImp::display(int id, render::VirtualDisplay& world)
		{
			const DisplayMap::iterator res = displays.find(id);
			if(res == displays.end())
			{
				throw "id not recognized";
			}

			Display* display = res->second;
			display->render(world);
		}

		void GameImp::display_remove(int id, Display* disp)
		{
			const DisplayMap::iterator res = displays.find(id);
			if(res == displays.end())
			{
				throw "id not previously added";
			}

			if(res->second != disp)
			{
				throw "id was not associated with disp";
			}

			displays[id] = 0;
		}

		void GameImp::handleKey(Key::Code key, bool isDown)
		{
			if(ActiveLoop::Has())
			{
				ActiveLoop::Get().onKey(key, isDown);
			}
		}

		void GameImp::handleMouse(const math::vec2 movement)
		{
			if(ActiveLoop::Has())
			{
				ActiveLoop::Get().onMouse(movement);
			}
		}
	}
}
