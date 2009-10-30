#include "videomode.hpp"
#include <SFML/Window.hpp>

namespace pwn
{
	namespace engine
	{
		VideoMode::VideoMode()
		{
		}

		VideoMode::VideoMode(int width, int height, int bits, bool fullscreen)
			: width(width)
			, height(height)
			, bits(bits)
			, fullscreen(fullscreen)
		{
		}

		bool VideoMode::isValid() const
		{
			sf::VideoMode vm(width, height, bits);
			return vm.IsValid();
		}

		namespace VideoModes
		{
			const VideoMode At(index i);
			const index Count();
		}
	}
}