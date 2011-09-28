#include <pwn/render/world3.h>
#include <pwn/render/actor.h>
#include <pwn/render/light.h>
#include <pwn/render/renderlist.h>
#include <pwn/render/RenderArgs.h>
#include <vector>
#include <algorithm>
#include <pwn/render/compiledcamera.h>
#include <boost/utility.hpp>

#pragma warning(disable:4512) // warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

#include "opengl_debug.hpp"

// for in
#include <pwn/math/operations.h>

namespace pwn
{
	namespace render
	{
		World3::World3()
		{
		}
		World3::~World3()
		{
		}

		class BasicWorld
			: public World3
			, boost::noncopyable
		{
		public:
			BasicWorld()
				: list(true)
				, ambient(0.2f, 0.2f, 0.2f, 1.0f)
			{
			}

			void actor_add(ActorPtr actor)
			{
				actors.push_back(actor);
			}

			void actor_remove(ActorPtr actor)
			{
				ActorList::iterator res = std::find(actors.begin(), actors.end(), actor);
				if( res == actors.end() ) return;
				actors.erase(res);
			}

			void light_add(LightPtr light)
			{
				lights.push_back(light);
			}

			void light_remove(LightPtr light)
			{
				LightList::iterator res = std::find(lights.begin(), lights.end(), light);
				if( res == lights.end() ) return;
				lights.erase(res);
			}

			void light_setAmbient(const math::Rgba& c)
			{
				ambient = c;
			}

			void render(const RenderArgs& r) const
			{
				glViewport(r.x, r.y, r.width, r.height);
				pwnAssert_NoGLError();

				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient.data());

				list.begin();
				list.add(r.compiled.generateMatrix(r.camera.position.vec + math::Left(r.camera.orientation)*4 + math::In(r.camera.orientation)*10, r.camera.orientation));
				BOOST_FOREACH(ActorPtr a, actors)
				{
					a->render(&list, r.compiled);
				}

				int id = GL_LIGHT0; pwnAssert_NoGLError();

				glLoadMatrixf( r.compiled.mat.data() );  pwnAssert_NoGLError();

				if( lights.empty() == false )
				{
					glEnable(GL_LIGHTING); pwnAssert_NoGLError();
					BOOST_FOREACH(LightPtr l, lights)
					{
						glEnable(id); pwnAssert_NoGLError();
						l->apply(id);
						++id;
					}
				}

				// todo: shouldnt we always enable materials? there might be ambiant light and should be really optimize for pure darkness?
				list.end( lights.empty() == false ); // apply materials if there are lights

				// rendering is done, disable all lights again
				for(int i=GL_LIGHT0; i<id; ++i)
				{
					glDisable(i);
				}
			}

			mutable RenderList list; // mutable since we want to use frame-to-frame coherence, and avoid uneccessary reallocation of its internals
			// cant be static, since it needs to be killed when the class is killed

			typedef std::vector<ActorPtr> ActorList;
			ActorList actors;

			typedef std::vector<LightPtr> LightList;
			LightList lights;

			math::Rgba ambient;
		};

		World3::Ptr World3::Create()
		{
			return Ptr(new BasicWorld());
		}
	}
}
