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
				RenderArgs(WorldPtr world, const CompiledCamera& cam, int width, int height);
				WorldPtr getWorld();
				const CompiledCamera& getCamera() const;
				int getWidth();
				int getHeight();
				
			public:
				void render() const;
				
			private:
				WorldPtr world;
				CompiledCamera cam;
				int width;
				int height;
				RenderArgs(const RenderArgs&);
			};
		}
	}
}

#endif
