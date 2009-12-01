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
#include <pwn/mesh/predefinedmaterials>
#include <pwn/engine/democamera>
#include <pwn/engine/vfstexturepool2>
#include <pwn/mesh/material>

using namespace pwn;
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
	{
		const rect res = FromUpperLeftAndSize(Origo2(), direction2(world2.getWidth(), world2.getHeight()));

		Mesh mesh;
		const pwn::real halfsize = 0.7f;
		SetCube(&mesh, materials::Plastic_White(), halfsize*2, halfsize*2, halfsize*2, true);
		mesh.materials.push_back( materials::Plastic_Green() );
		mesh.triangles[0].material = 1;
		mesh.materials[0]->texture = "crate01a.jpg";
		//BuildNormals(&mesh);
		Move(&mesh, vec3(-halfsize, -halfsize, -halfsize));
		def = Compile(mesh, &tpool);
		
		act.reset( new Actor(point3(0,0, 0), qIdentity()) );
		act->model = def;

		boost::shared_ptr<World3> world( World3::Create() );
		world->actor_add(act);

		{
			Mesh mesh;
			const pwn::real h = 200;
			SetCube(&mesh, materials::Pearl(), h*2, h*2, h*2, false);
			//BuildNormals(&mesh);
			Move(&mesh, vec3(-h, -h,-h));
			boost::shared_ptr<Actor> act( new Actor(Origo3(), qIdentity()) );
			act->model = Compile(mesh, &tpool);
			world->actor_add(act);
		}

		boost::shared_ptr<World3Widget > wid( new World3Widget( FromAspectAndContainingInCenter(res, 14.0f/9.0f), world ) ); // http://en.wikipedia.org/wiki/14:9

		dcam.world = wid;

		world2.widget_add( wid );
	}

	void onKey(Key::Code key, bool isDown)
	{
		if( key == Key::Escape && isDown )
		{
			stop();
		}
		else
		{
			dcam.onKey(key, isDown);
		}
	}

	void onUpdate(real delta)
	{
		dcam.update(delta, 3.0f, 10.0f);
	}

	void onRender()
	{
		renderWorld(0, world2);
	}

	void onMouse(const math::vec2 movement)
	{
		dcam.onMouse(movement);
	}

	World2 world2;
	boost::shared_ptr<ActorDef> def;
	boost::shared_ptr<Actor> act;
	DemoCamera dcam;
	VfsTexturePool2 tpool;
};

int main(int, char** argv)
{
	Game game;
	InstallDefaultSystems(&game, argv[0], "entsoft", "survivors", "Survivors!");
	EasyLoop(&game).loop();
}