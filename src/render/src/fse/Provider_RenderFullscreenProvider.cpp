#include <pwn/render/fse/Provider_RenderFullScreenProvider.h>
#include <pwn/render/fse/Target.h>
//#include <pwn/render/fse/Binder.h>
#include <pwn/render/utils.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			RenderFullscreenProvider::RenderFullscreenProvider(const core::Ptree& el)
				: Provider(el)
			{
			}

			RenderFullscreenProvider::~RenderFullscreenProvider()
			{
			}

			void RenderFullscreenProvider::doProvide(World3* world, const RenderArgs& ra)
			{
				if(shader.get()) Shader::Bind(shader);
				callCommands(); // lets call the commands
				RenderFullscreenQuad(0, getTarget()->getWidth(), getTarget()->getHeight());
				if (shader.get()) Shader::Unbind(shader);
			}

			void RenderFullscreenProvider::doLink(Linker* user)
			{
				denyAutocallOfCommands(); // call the commands ourself
			}

			void RenderFullscreenProvider::doBind(Binder* bd)
			{
			}

			const string RenderFullscreenProvider::toString() const
			{
				// todo: add shader to description?
				return Provider::toString() + " renders fullscreen with " + ( shader.get()?"shader":" no shader" );
			}
		}
	}
}
