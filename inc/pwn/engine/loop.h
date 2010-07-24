#ifndef PWN_ENGINE_LOOP
#define PWN_ENGINE_LOOP

#include <pwn/engine/key.h>
#include <pwn/math/types.h>

namespace pwn
{
	namespace render
	{
		class VirtualDisplay;
	}

	namespace engine
	{
		class Game;

		class Loop
		{
		public:
			Loop(Game* game);
			virtual ~Loop();

			void loop();

			virtual void onKey(Key::Code key, bool isDown) = 0;
			virtual void onMouse(const math::vec2 movement) = 0;

			bool isRunning() const;
		protected:
			void stop();
			Game& game();

			virtual void onUpdate(real delta) = 0;
			virtual void onRender() = 0;

			void renderWorld(int id, render::VirtualDisplay& world);
		private:
			void update(real delta);
			void render();
			bool isRunning_;
			Game* game_;
		};
	}
}

#endif