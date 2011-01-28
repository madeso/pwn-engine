#ifndef PWN_RENDER_FSE_RENDERARGS
#define PWN_RENDER_FSE_RENDERARGS

#include <pwn/render/camera.h>
#include <pwn/render/compiledcamera.h>

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
			RenderArgs(const Camera& cam, int x, int y, int width, int height, real aaspect); ///< set all the values
			RenderArgs(const RenderArgs& ra, int width, int height); ///< get the camera from the other RenderArgs and render w/ width, height at 0,0

		public:
			void render(World3* world) const;

			CompiledCamera compiled;
			Camera camera;
			int x;
			int y;
			int width;
			int height;
			real aspect;
		private:
			RenderArgs(const RenderArgs&);
		};
	}
}

#endif
