#ifndef PWN_RENDER_FSE_PROVIDER_RENDERFULLSCREENPROVIDER
#define PWN_RENDER_FSE_PROVIDER_RENDERFULLSCREENPROVIDER

#include <pwn/render/fse/Provider.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class RenderFullscreenProvider
				: public Provider
			{
			public:
				RenderFullscreenProvider(const core::Ptree& el, const string& file);
				~RenderFullscreenProvider();

			protected:
				void doProvide(World3* world, const RenderArgs& ra);
				void doLink(Linker* user);
				void doBind(Binder* bd);

				const string toString() const;
			};
		}
	}
}

#endif
