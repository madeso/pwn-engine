#include <pwn/render/worldwithcameraboundobject3.h>
#include <pwn/render/actor.h>
#include <pwn/render/camera.h>
#include <pwn/render/compiledcamera.h>
#include <pwn/render/renderlist.h>
#include <pwn/render/renderargs.h>
#include <pwn/math/operations.h>
#include "opengl_debug.hpp"

namespace pwn
{
	namespace render
	{
		WorldWithCameraBoundObject3::WorldWithCameraBoundObject3(ActorPtr a, World3::Ptr w)
			: WorldWith3(w)
			, actor(a)
		{
		}

		Camera MoveToOrigo(const Camera& camera)
		{
			Camera c = camera;
			c.position = math::Origo3();
			return c;
		}

		void WorldWithCameraBoundObject3::render(const RenderArgs& r) const
		{
			RenderList list(true); // todo: move to pimpl or provide a render direct interface to the actor..?
			CompiledCamera cc(MoveToOrigo(r.camera));
			list.begin();
			actor->render(&list, cc);
			list.end(false);

			pwnAssert_NoGLError();
			glClear(GL_DEPTH_BUFFER_BIT);
			pwnAssert_NoGLError();

			WorldWith3::render(r);
		}
	}
}
