#ifndef PWN_RENDER_WORLD3
#define PWN_RENDER_WORLD3

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class Actor;
		class CompiledCamera;

		class World3
		{
		public:
			World3();
			virtual ~World3();

			typedef boost::shared_ptr<Actor> ActorPtr;

			virtual void actor_add(ActorPtr actor) = 0;
			virtual void actor_remove(ActorPtr actor) = 0;

			virtual void render(int x, int y, int w, int h, const CompiledCamera& cc) = 0;

			typedef boost::shared_ptr<World3> Ptr;

			static Ptr Create();
		};
	}
}

#endif