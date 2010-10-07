#include <pwn/render/worldwithcameraboundobject3.h>
#include <pwn/render/actor.h>
#include <pwn/render/camera.h>
#include <pwn/render/compiledcamera.h>
#include <pwn/render/renderlist.h>
#include <pwn/math/operations.h>
#include <SFML/OpenGl.hpp>
#include "opengl_debug.hpp"
#include <pwn/assert.h>

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

		void WorldWithCameraBoundObject3::render(int x, int y, int w, int h, const CompiledCamera& cc) const
		{
			RenderList list(true); // todo: move to pimpl or provide a render direct interface to the actor..?
			list.begin();
			actor->render(&list, cc);
			list.end();
			
			Assert( glGetError_WithString() == GL_NO_ERROR);
			glClear(GL_DEPTH_BUFFER_BIT);
			Assert( glGetError_WithString() == GL_NO_ERROR);

			WorldWith3::render(x, y, w, h, cc);
		}
	}
}