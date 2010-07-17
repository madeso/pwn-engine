#ifndef PWN_RENDER_ACTOR
#define PWN_RENDER_ACTOR

#include <pwn/math/types>

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class ActorDef;
		class RenderList;
		class CompiledCamera;

		/** A animated mesh instance in the World3.
		 */
		class Actor
		{
		public:
			Actor(const math::point3& location, const math::quat& rotation, boost::shared_ptr<ActorDef> model);

			static boost::shared_ptr<Actor> Create(const math::point3& location, const math::quat& rotation, boost::shared_ptr<ActorDef> model);

			void render(RenderList* rl, const CompiledCamera& camera);

			boost::shared_ptr<ActorDef> model;
			math::point3 location;
			math::quat rotation;
		};
	}
}

#endif