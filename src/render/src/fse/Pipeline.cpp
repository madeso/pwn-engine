#include <pwn/render/fse/Pipeline.h>
#include <pwn/render/fse/Binder.h>
#include <pwn/render/fse/Provider.h>
#include <pwn/render/fse/Target.h>
#include <boost/foreach.hpp>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			PipelinePtr Pipeline::Create(const string& path, int width, int height)
			{
				PipelinePtr pp (new Pipeline());
				string t = pp->linker.read(path, width, height);
				pp->linker.link();
				pp->linker.storePipeline(pp, pp->linker.getTarget(t) );
				Binder bind(&pp->linker);
				pp->bind(&bind);
				bind.createBuffers();
				return pp;
			}
			
			void Pipeline::render(WorldPtr world, const RenderArgs& ra)
			{
				BOOST_FOREACH(Provider* p, providers)
				{
					p->provide(world, ra);
				}
			}
			
			Pipeline::Pipeline()
			{
			}

			Pipeline::~Pipeline()
			{
			}
			
			void Pipeline::add(Provider* pr)
			{
				std::vector<Provider*> prp = pr->getProviders();
				BOOST_FOREACH(Provider* p, prp)
				{
					add(p);
				}
				
				if (providers.end() == find(providers.begin(), providers.end(), pr))
				{
					providers.push_back(pr);
				}
			}
			
			void Pipeline::bind(Binder* binder)
			{
				BOOST_FOREACH(Provider* p, providers)
				{
					p->bind(binder);
					if (p->getTarget().get() != 0) // move this to the provider?
					{
						p->getTarget()->bind(binder);
					}
				}
			}
		}
	}
}
