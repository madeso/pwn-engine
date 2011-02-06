#ifndef PWN_RENDER_ENGINE
#define PWN_RENDER_ENGINE

#include <pwn/number.h>

namespace pwn
{
	namespace render
	{
		class Engine
		{
		public:
			Engine();
			~Engine();

			real getAnistropy() const;
		};
	}
}

#endif
