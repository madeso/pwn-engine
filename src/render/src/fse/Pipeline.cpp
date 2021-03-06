﻿#include <pwn/render/fse/Pipeline.h>
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
        PipelinePtr
        Pipeline::Create(const string& path, ShaderPool* shaders)
        {
            PipelinePtr pp(new Pipeline());
            string t = pp->linker.read(path, shaders);
            pp->linker.link();
            pp->linker.storePipeline(pp, pp->linker.getTarget(t));
            Binder bind(&pp->linker, shaders);
            pp->bind(&bind, shaders);
            bind.createBuffers();
            return pp;
        }

        void
        Pipeline::render(World3* world, const RenderArgs& ra)
        {
            BOOST_FOREACH (Provider* p, providers)
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

        void
        Pipeline::add(Provider* pr)
        {
            std::vector<Provider*> prp = pr->getProviders();
            BOOST_FOREACH (Provider* p, prp)
            {
                add(p);
            }

            if (providers.end() == find(providers.begin(), providers.end(), pr))
            {
                providers.push_back(pr);
            }
        }

        void
        Pipeline::bind(Binder* binder, ShaderPool* shaders)
        {
            BOOST_FOREACH (Provider* p, providers)
            {
                p->bind(binder, shaders);
                if (p->getTarget().get() != 0)  // move this to the provider?
                {
                    p->getTarget()->bind(binder);
                }
            }
        }
    }
}
}
