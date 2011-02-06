#ifndef PWN_RENDER_DOCK
#define PWN_RENDER_DOCK

#include <pwn/math/types.h>

namespace pwn
{
	namespace render
	{
		/** determines how a Widget is displayed.
		*/
		class Dock
		{
		public:
			/// the Widget fills up the entire VirtualDisplay.
			static Dock Fill();

			math::rect getRect(int width, int height) const;
		private:
			Dock();
		};
	}
}

#endif
