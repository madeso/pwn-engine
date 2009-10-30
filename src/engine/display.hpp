#ifndef PWN_ENGINE_WINDOW_HPP
#define PWN_ENGINE_WINDOW_HPP

namespace pwn
{
	namespace engine
	{
		class Display
		{
		public:
			Display();
			virtual ~Display();

			virtual void render();
		};
	}
}

#endif