#ifndef PWN_ENGINE_SYSTEM_HPP
#define PWN_ENGINE_SYSTEM_HPP

namespace pwn
{
	namespace engine
	{
		class System
		{
		public:
			System();
			virtual ~System();

			virtual void update() = 0;
		private:
			System(const System&);
			System& operator=(const System&);
		};
	}
}

#endif
