#include <pwn/engine/loop>
#include <pwn/engine/game>

#include <pwn/render/world2>
#include <pwn/render/world3widget>
#include <pwn/math/operations>

using namespace pwn::engine;
using namespace pwn::render;
using namespace pwn::math;

class EasyLoop : public Loop
{
public:
	EasyLoop(Game* game)
		: Loop(game)
		, world(800, 600)
	{
		const rect res = FromUpperLeftAndSize(Origo2(), direction2(world.getWidth(), world.getHeight()));
		world.widget_add( new World3Widget( FromAspectAndContainingInCenter(res, 1.618f) ) ); // golden-ratio 1.618f
	}

	void onKey(Key::Code key, bool isDown)
	{
		if( key == Key::Escape && isDown )
		{
			stop();
		}
	}

	void onUpdate()
	{
	}

	void onRender()
	{
		renderWorld(0, world);
	}

	World2 world;
};

int main(int, char** argv)
{
	Game game;
	InstallDefaultSystems(&game, argv[0], "entsoft", "survivors", "Survivors!");
	EasyLoop(&game).loop();
}