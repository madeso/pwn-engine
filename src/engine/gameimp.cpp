#include "gameimp.hpp"
#include "system.hpp"
#include <algorithm>

namespace pwn
{
	namespace engine
	{
		GameImp::GameImp()
		{
		}

		GameImp::~GameImp()
		{
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
			if( res != displays.end() && res->second != 0 )
			{
				throw "id belongs to someone else";
			}
			displays[id] = disp;
		}

		void GameImp::display_remove(int id, Display* disp)
		{
			const DisplayMap::iterator res = displays.find(id);
			if( res == displays.end() )
			{
				throw "id not previously added";
			}

			if( res->second != disp )
			{
				throw "id was not associated with disp";
			}

			displays[id] = 0;
		}
	}
}