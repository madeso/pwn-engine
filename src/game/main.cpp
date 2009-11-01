#include <pwn/engine/loop>
#include <pwn/engine/game>

using namespace pwn::engine;

class EasyLoop : public Loop
{
public:
	EasyLoop(Game* game)
		: Loop(game)
	{
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
		renderWorld(0);
	}
};

int main(int, char** argv)
{
	Game game;
	InstallDefaultSystems(&game, argv[0], "entsoft", "survivors", "Survivors!");
	EasyLoop(&game).loop();
}