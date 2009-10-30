#include <pwn/engine/loop>
#include <pwn/engine/Game>
#include "GameImp.hpp"

namespace pwn
{
	namespace engine
	{
		Loop::Loop(Game* game)
			: isRunning_(false)
			, game(game)
		{
		}

		Loop::~Loop()
		{
		}

		void Loop::stop()
		{
			isRunning_ = false;
		}

		bool Loop::isRunning() const
		{
			return isRunning_;
		}

		namespace // local
		{
			Loop*& InnerMostLoop()
			{
				static Loop* loop = 0;
				return loop;
			}

			class RaiiInnerMostLoop
			{
			public:
				explicit RaiiInnerMostLoop(Loop* me)
					: old( InnerMostLoop() )
				{
					InnerMostLoop() = me;
				}
				~RaiiInnerMostLoop()
				{
					InnerMostLoop() = old;
				}

			private:
				Loop* old;
			};
		}

		void Loop::loop()
		{
			RaiiInnerMostLoop raiiInnerMostLoop(this);
			while(isRunning())
			{
				update();
				render();
			}
		}

		void Loop::update()
		{
			game->getImp().updateSystems();
		}

		void Loop::render()
		{
			if( isRunning() == false ) return;
		}
	}
}