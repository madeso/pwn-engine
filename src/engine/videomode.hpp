#ifndef PWN_ENGINE_SYSTEM_WINDOW_HPP
#define PWN_ENGINE_SYSTEM_WINDOW_HPP

#include <vector>

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
			const std::vector<VideoMode> List();
			const VideoMode Desktop();
		}
	}
}

#endif