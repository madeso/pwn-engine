#include <pwn/render/fse/Provider_RenderWorldProvider.h>
#include <pwn/render/fse/Binder.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			RenderWorldProvider::RenderWorldProvider(const core::Ptree& e)
				: Provider(e)
			{
			}

			RenderWorldProvider::~RenderWorldProvider()
			{
			}
			
			void RenderWorldProvider::doProvide(World3* world, const RenderArgs& ra)
			{
				Shader::Bind(shader);
				ra.render(world);
				Shader::Unbind(shader);
			}
			
			void RenderWorldProvider::doLink(Linker*)
			{
			}
			
			void RenderWorldProvider::doBind(Binder*)
			{
			}
			
			const string RenderWorldProvider::toString() const
			{
				return Provider::toString() + " renders world with " + ((shader.get())?"shader":"no shader");
			}
		}
	}
}
