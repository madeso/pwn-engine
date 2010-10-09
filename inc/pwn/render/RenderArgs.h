#ifndef PWN_RENDER_FSE_RENDERARGS
#define PWN_RENDER_FSE_RENDERARGS

#include <pwn/render/Camera.h>
#include <pwn/render/CompiledCamera.h>

namespace pwn
{
	namespace render
	{
		class World3;

		/** Basic class that are sent around containing arguments for rendering.
		*/
		class RenderArgs
			: boost::noncopyable
		{
		public:
			RenderArgs(const Camera& cam, int x, int y, int width, int height);

		public:
			void render(World3* world) const;
		
			CompiledCamera compiled;
			Camera camera;
			int x;
			int y;
			int width;
			int height;
		private:
			RenderArgs(const RenderArgs&);
		};
	}
}

#endif
