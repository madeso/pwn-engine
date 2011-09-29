#ifndef PWN_RENDER_ACTOR
#define PWN_RENDER_ACTOR

#include <pwn/math/types.h>
#include <pwn/render/poseable.h>
#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace mesh
	{
		class Pose;
	}

	namespace render
	{
		class ActorDef;
		class RenderList;
		class CompiledCamera;

		/** A animated mesh instance in the World3.
		 */
		class Actor
			: public Poseable
		{
		public:
			Actor(const math::point3& position, const math::quat& rotation, boost::shared_ptr<ActorDef> model);

			static boost::shared_ptr<Actor> Create(const math::point3& position, const math::quat& rotation, boost::shared_ptr<ActorDef> model);

			void render(RenderList* rl, const CompiledCamera& camera);

			void setPose(const mesh::Pose& p);

			boost::shared_ptr<ActorDef> model;
			math::point3 position;
			math::quat rotation;
		};
	}
}

#endif
