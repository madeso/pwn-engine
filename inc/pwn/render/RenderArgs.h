#ifndef PWN_RENDER_FSE_RENDERARGS
#define PWN_RENDER_FSE_RENDERARGS

#include <pwn/render/fse/types.h>
#include <pwn/render/CompiledCamera.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class RenderArgs
				: boost::noncopyable
			{
			public:
				RenderArgs(const CompiledCamera& cam, int width, int height);
				const CompiledCamera& getCamera() const;
				int getWidth();
				int getHeight();
				
			public:
				void render(WorldPtr world) const;
				
			private:
				CompiledCamera cam;
				int width;
				int height;
				RenderArgs(const RenderArgs&);
			};
		}
	}
}

#endif
