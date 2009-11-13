#ifndef PWN_RENDER_WORLD3
#define PWN_RENDER_WORLD3

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class Actor;
		class Camera;

		class World3
		{
		public:
			World3();
			virtual ~World3();

			typedef boost::shared_ptr<Actor> ActorPtr;

			virtual void actor_add(ActorPtr actor) = 0;
			virtual void actor_remove(ActorPtr actor) = 0;

			virtual void render(const Camera& camera) = 0;

			static World3* Create();
		};
	}
}

#endif