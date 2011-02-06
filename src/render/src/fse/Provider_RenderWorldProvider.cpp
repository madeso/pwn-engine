#include <pwn/render/fse/Provider_RenderWorldProvider.h>
#include <pwn/render/fse/Binder.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			RenderWorldProvider::RenderWorldProvider(const core::Ptree& e, const string& file)
				: Provider(e, file)
			{
			}

			RenderWorldProvider::~RenderWorldProvider()
			{
			}

			void RenderWorldProvider::doProvide(World3* world, const RenderArgs& ra)
			{
				// uncommented to make sure simple vertex shaders arent interfering with animation applying
				//Shader::Bind(shader);
				ra.render(world);
				//Shader::Unbind(shader);
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
