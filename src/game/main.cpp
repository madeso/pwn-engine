#include <pwn/engine/loop>
#include <pwn/engine/game>

#include <pwn/render/world2>
#include <pwn/render/world3>
#include <pwn/render/world3widget>
#include <pwn/math/operations>
#include <pwn/render/actordef>
#include <pwn/render/actor>
#include <pwn/mesh/mesh>
#include <pwn/mesh/builder>

using namespace pwn::engine;
using namespace pwn::render;
using namespace pwn::math;
using namespace pwn::mesh;

class EasyLoop : public Loop
{
public:
	EasyLoop(Game* game)
		: Loop(game)
		, world2(800, 600)
		, act(point3(-1.5,0, -12), qIdentity())
	{
		const rect res = FromUpperLeftAndSize(Origo2(), direction2(world2.getWidth(), world2.getHeight()));

		Mesh mesh;
		const pwn::real halfsize = 0.7;
		SetCube(&mesh, halfsize*2, halfsize*2, halfsize*2);
		BuildNormals(&mesh);
		Move(&mesh, vec3(-halfsize, -halfsize, -halfsize));
		def = Compile(mesh);
		act.model = def.get();

		boost::shared_ptr<World3> world( World3::Create() );
		world->actor_add(&act);

		world2.widget_add( new World3Widget( FromAspectAndContainingInCenter(res, 1.618f), world ) ); // golden-ratio 1.618f
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
		renderWorld(0, world2);
	}

	World2 world2;
	boost::shared_ptr<ActorDef> def;
	Actor act;
};

int main(int, char** argv)
{
	Game game;
	InstallDefaultSystems(&game, argv[0], "entsoft", "survivors", "Survivors!");
	EasyLoop(&game).loop();
}