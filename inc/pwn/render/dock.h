#ifndef PWN_RENDER_DOCK
#define PWN_RENDER_DOCK

#include <pwn/math/types.h>

namespace pwn
{
	namespace render
	{
		class Dock
		{
		public:
			static Dock Fill();

			math::rect getRect(int width, int height) const;
		private:
			Dock();
		};
	}
}

#endif