#include <pwn/render/world3.h>
#include <pwn/render/actor.h>
#include <pwn/render/renderlist.h>
#include <vector>
#include <algorithm>
#include <pwn/render/compiledcamera.h>
#include <boost/utility.hpp>

#pragma warning(disable:4512) // warning C4512: 'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>
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

			void render(const Camera& camera)
			{
				const CompiledCamera cc(camera);
				list.begin();
				BOOST_FOREACH(ActorPtr a, actors)
				{
					a->render(&list, cc);
				}
				list.end();
			}

			RenderList list;
			typedef std::vector<ActorPtr> ActorList;
			ActorList actors;
		};

		World3::Ptr World3::Create()
		{
			return Ptr(new BasicWorld());
		}
	}
}