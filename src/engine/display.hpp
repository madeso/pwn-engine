#ifndef PWN_ENGINE_WINDOW_HPP
#define PWN_ENGINE_WINDOW_HPP

namespace pwn
{
	namespace engine
	{
		/** basic interface for diplaying something on-screen
		*/
		class Display
		{
		public:
			Display();
			virtual ~Display();

			virtual void render() = 0;
		};
	}
}

#endif