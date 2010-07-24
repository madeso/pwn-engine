#include <pwn/engine/loop.h>
#include <pwn/engine/game.h>
#include <pwn/engine/startup.h>

#include <pwn/render/virtualdisplay.h>
#include <pwn/render/world3.h>
#include <pwn/render/world3widget.h>
#include <pwn/math/operations.h>
#include <pwn/render/actordef.h>
#include <pwn/render/actor.h>
#include <pwn/mesh/mesh.h>
#include <pwn/mesh/builder.h>
#include <pwn/mesh/predefinedmaterials.h>
#include <pwn/engine/democamera.h>
#include <pwn/engine/vfstexturepool2.h>
#include <pwn/mesh/material.h>
#include <pwn/meshio/io.h>

using namespace pwn;
using namespace pwn::engine;
using namespace pwn::render;
using namespace pwn::math;
using namespace pwn::mesh;

boost::shared_ptr<ActorDef> CreateCube(real size, const string& texture, TexturePool2* tpool, real alpha, bool out)
{
	Mesh mesh;
	const real halfsize = size/2;

	SetBox(&mesh, materials::White(), halfsize*2, halfsize*2, halfsize*2, out);
	mesh.materials[0]->texture_diffuse = texture;
	mesh.materials[0]->diffuse.alpha(alpha);
	Move(&mesh, vec3(-halfsize, -halfsize, -halfsize));
	return Compile(mesh, tpool);
}

boost::shared_ptr<ActorDef> LoadMesh(const string& file, TexturePool2* tpool)
{
	Mesh mesh;
	pwn::meshio::Read(&mesh, file);
	return Compile(mesh, tpool);
}

class EasyLoop : public Loop
{
public:
	EasyLoop(Game* game)
		: Loop(game)
		, display(800, 600)
	{
		const rect res = FromUpperLeftAndSize(Origo2(), direction2(display.getWidth(), display.getHeight()));

		boost::shared_ptr<World3> world( World3::Create() );

		world->actor_add( Actor::Create(point3(0,0,0), qIdentity(), LoadMesh("fighter1.mesh", &tpool)) );
		world->actor_add( Actor::Create(Origo3(), qIdentity(), CreateCube(400, "_stars-texture.jpg", &tpool, 1, false) ) ); // sky texture

		boost::shared_ptr<World3Widget > wid( new World3Widget( FromAspectAndContainingInCenter(res, 14.0f/9.0f), world ) ); // http://en.wikipedia.org/wiki/14:9

		dcam.world = wid;

		display.widget_add( wid );
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
		renderWorld(0, display);
	}

	void onMouse(const math::vec2 movement)
	{
		dcam.onMouse(movement);
	}

	VirtualDisplay display;
	DemoCamera dcam;
	VfsTexturePool2 tpool;
};

int main(int, char** argv)
{
	{
		Game game;
		InstallDefaultSystems(&game,
			Startup(argv[0], "entsoft", "survivors", "Survivors!")
			);
		EasyLoop(&game).loop();
	}
}