#define PWN_ENGINE_NO_AUTOLINK

#include <pwn/engine/loop>
#include <pwn/engine/Game>
#include "GameImp.hpp"
#include "loop.hpp"


namespace pwn
{
	namespace engine
	{
		Loop::Loop(Game* game)
			: isRunning_(false)
			, game_(game)
		{
		}

		Loop::~Loop()
		{
		}

		Game& Loop::game()
		{
			if( game_ == 0 ) throw "invalid game variable";
			return *game_;
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

		namespace ActiveLoop
		{
			Loop& Get()
			{
				Loop* loop = InnerMostLoop();
				if( loop == 0 ) throw "assertion";
				return *loop;
			}
			bool Has()
			{
				return InnerMostLoop() != 0;
			}
		}

		void Loop::loop()
		{
			RaiiInnerMostLoop raiiInnerMostLoop(this);
			isRunning_ = true;
			while(isRunning())
			{
				update();
				render();
			}
		}

		void Loop::update()
		{
			if( isRunning() == false ) return;
			game().getImp().updateSystems();

			if( isRunning() == false ) return;
			onUpdate();
		}

		void Loop::render()
		{
			if( isRunning() == false ) return;
			onRender();
		}

		void Loop::renderWorld(int id)
		{
			game().getImp().display(id);
		}
	}
}