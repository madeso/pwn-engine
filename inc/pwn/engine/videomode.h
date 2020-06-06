#ifndef PWN_ENGINE_VIDEOMODE
#define PWN_ENGINE_VIDEOMODE

#include <vector>

namespace pwn
{
	namespace engine
	{
		struct VideoMode
		{
			VideoMode(); // doesn't do anything
			VideoMode(int width, int height, int bits, bool fullscreen = false);
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
