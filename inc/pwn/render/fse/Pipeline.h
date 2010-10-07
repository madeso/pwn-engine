#ifndef PWN_RENDER_FSE_PIPELINE
#define PWN_RENDER_FSE_PIPELINE

#include <pwn/render/fse/types.h>
#include <pwn/string.h>
#include <pwn/render/fse/RenderArgs.h>
#include <vector>
#include <pwn/render/fse/Linker.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class Pipeline
			{
			public:
				static PipelinePtr Create(const string& path, int width, int height);
				void render(WorldPtr world, const RenderArgs& ra);

			public:
				Pipeline();
				~Pipeline();
				void add(Provider* pr);
				
			private:
				void bind(Binder* binder);
				
			private:
				Linker linker;
				std::vector<Provider*> providers;
			};
		}
	}
}

#endif
