#ifndef PWN_RENDER_FSE_PIPELINE
#define PWN_RENDER_FSE_PIPELINE

#include <pwn/render/fse/types.h>
#include <pwn/string.h>
#include <pwn/render/RenderArgs.h>
#include <vector>
#include <pwn/render/fse/Linker.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			/** Entry point and probably the only class of interest when using the fse framework.
			*/
			class Pipeline
			{
			public:
				static PipelinePtr Create(const string& path, int width, int height);
				void render(World3* world, const RenderArgs& ra);
				~Pipeline();

			public:
				void add(Provider* pr); ///< internal
				
			private:
				Pipeline();
				void bind(Binder* binder);
				
			private:
				Linker linker;
				std::vector<Provider*> providers;
			};
		}
	}
}

#endif
