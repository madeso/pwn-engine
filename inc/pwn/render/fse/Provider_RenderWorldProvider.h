#ifndef PWN_RENDER_FSE_PROVIDER_RENDERWORLDPROVIDER
#define PWN_RENDER_FSE_PROVIDER_RENDERWORLDPROVIDER

#include <pwn/render/fse/Provider.h>
#include <pwn/render/shader.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class RenderWorldProvider
				: public Provider
				, boost::noncopyable
			{
			public:
				RenderWorldProvider(const core::Ptree& e);
				~RenderWorldProvider();
				const string toString() const;

			protected:
				void doProvide(const RenderArgs& ra);
				void doLink(Linker* user);
				void doBind(Binder* bd);
			};
		}
	}
}

#endif