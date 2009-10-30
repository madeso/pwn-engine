#ifndef PWN_ENGINE_SYSTEM_WINDOW_HPP
#define PWN_ENGINE_SYSTEM_WINDOW_HPP

namespace pwn
{
	namespace engine
	{
		struct VideoMode
		{
			VideoMode(); // doesn't do anything
			VideoMode(int width, int height, int bits, bool fullscreen=false);
			int width;
			int height;
			int bits;
			bool fullscreen;

			bool isValid() const;
		};

		namespace VideoModes
		{
			typedef unsigned int index;
			const VideoMode At(index i);
			const index Count();
		}
	}
}

#endif