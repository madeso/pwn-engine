#ifndef PWN_RENDER_WORLD3
#define PWN_RENDER_WORLD3

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class Actor;
		class Light;
		class RenderArgs;

		/** A world is a list of actors to be rendered.
		*/
		class World3
		{
		public:
			World3();
			virtual ~World3();

			typedef boost::shared_ptr<Actor> ActorPtr;
			typedef boost::shared_ptr<Light> LightPtr;

			virtual void actor_add(ActorPtr actor) = 0;
			virtual void actor_remove(ActorPtr actor) = 0;

			virtual void light_add(LightPtr light) = 0;
			virtual void light_remove(LightPtr light) = 0;

			virtual void render(const RenderArgs& r) const = 0;

			typedef boost::shared_ptr<World3> Ptr;

			static Ptr Create();
		};
	}
}

#endif