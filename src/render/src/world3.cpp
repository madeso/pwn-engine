#include <pwn/render/world3.h>
#include <pwn/render/actor.h>
#include <pwn/render/renderlist.h>
#include <pwn/render/renderargs.h>
#include <vector>
#include <algorithm>
#include <pwn/render/compiledcamera.h>
#include <boost/utility.hpp>

#pragma warning(disable:4512) // warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

#include <pwn/assert.h>

#include <SFML/OpenGl.hpp>
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

			void render(const RenderArgs& r) const
			{
				glViewport(r.x, r.y, r.width, r.height);
				Assert( glGetError_WithString() == GL_NO_ERROR);


				list.begin();
				BOOST_FOREACH(ActorPtr a, actors)
				{
					a->render(&list, r.compiled);
				}
				list.end();
			}

			mutable RenderList list; // mutable since we want to use frame-to-frame coherence, and avoid uneccessary reallocation of its internals
			// cant be static, since it needs to be killed when the class is killed

			typedef std::vector<ActorPtr> ActorList;
			ActorList actors;
		};

		World3::Ptr World3::Create()
		{
			return Ptr(new BasicWorld());
		}
	}
}