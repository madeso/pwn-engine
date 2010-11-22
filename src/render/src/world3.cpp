#include <pwn/render/world3.h>
#include <pwn/render/actor.h>
#include <pwn/render/light.h>
#include <pwn/render/renderlist.h>
#include <pwn/render/renderargs.h>
#include <vector>
#include <algorithm>
#include <pwn/render/compiledcamera.h>
#include <boost/utility.hpp>

#pragma warning(disable:4512) // warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

#include "opengl_debug.hpp"

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

			void render(const RenderArgs& r) const
			{
				glViewport(r.x, r.y, r.width, r.height);
				pwnAssert_NoGLError();

				list.begin();
				BOOST_FOREACH(ActorPtr a, actors)
				{
					a->render(&list, r.compiled);
				}

				int id = GL_LIGHT0; pwnAssert_NoGLError();

				glLoadMatrixf( r.compiled.mat.columnMajor );  pwnAssert_NoGLError();

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

				list.end( lights.empty() == false ); // apply materials if there are lightds

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
		};

		World3::Ptr World3::Create()
		{
			return Ptr(new BasicWorld());
		}
	}
}