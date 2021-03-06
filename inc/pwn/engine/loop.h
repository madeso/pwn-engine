#ifndef PWN_ENGINE_LOOP
#define PWN_ENGINE_LOOP

#include <pwn/engine/key.h>
#include <pwn/math/types.h>
#include <pwn/render/virtualdisplay.h>

namespace pwn
{
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

			/** Rendering callback.
			Here you tell pwn what to render. Usually you just want to render a single display,
			however in in-game menues you may want to render the calling loops display before you render the current.
			*/
			virtual void onRender() = 0;

			void defaultRender();

			void renderDisplay(render::VirtualDisplay& display, int id = 0);

			render::VirtualDisplay display;
		private:
			void update(real delta);
			void render();
			bool isRunning_;
			Game* game_;
		};
	}
}

#endif
