#ifndef PWN_RENDER_WORLDWITH3
#define PWN_RENDER_WORLDWITH3

#include <pwn/render/world3.h>

namespace pwn
{
	namespace render
	{
		/** WorldWith3 enables a faux inheritance from an unknown subclass of world.
		*/
		class WorldWith3 : public World3
		{
		public:
			WorldWith3(World3::Ptr w);
			virtual ~WorldWith3();

			virtual void actor_add(ActorPtr actor);
			virtual void actor_remove(ActorPtr actor);

			virtual void lines_add(LinesPtr lines);
			virtual void lines_remove(LinesPtr lines);

			virtual void light_add(LightPtr light);
			virtual void light_remove(LightPtr light);
			virtual void light_setAmbient(const math::Rgba& ambient);

			virtual void render(const RenderArgs& r) const;
		private:
			World3::Ptr world;
		};
	}
}

#endif
