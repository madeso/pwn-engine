#ifndef PWN_ENGINE_GAMEIMP_HPP
#define PWN_ENGINE_GAMEIMP_HPP

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

//#include <events/event.h>
#include <pwn/engine/key>

namespace pwn
{
	namespace engine
	{
		class System;
		class Display;

		/** Implementation of the Game class.
		* This class sort of follows the pimpl idiom, except the "private" is private at librariy-scope, and not at class-scope
		*/
		class GameImp
		{
		public:
			GameImp();
			~GameImp();
			void install(System* system); // assumes ownership
			void updateSystems();

			// only associate, ownership has to be handled somewhere else
			void display_add(int id, Display* disp);
			void display(int id);
			void display_remove(int id, Display* disp);

			// post events
			void handleKey(Key::Code key, bool isDown);

			typedef boost::shared_ptr<System> SystemPtr;
		private:
			std::vector<SystemPtr> systems;
			typedef std::map<int, Display*> DisplayMap;
			DisplayMap displays;
		};
	}
}

#endif