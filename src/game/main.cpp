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

boost::shared_ptr<ActorDef> CreateCube(real size, const string& texture, TexturePool2* tpool, real alpha, bool out)
{
	Mesh mesh;
	const real halfsize = size/2;

	SetBox(&mesh, materials::White(), halfsize*2, halfsize*2, halfsize*2, out);
	mesh.materials[0]->texture = texture;
	mesh.materials[0]->diffuse.alpha(alpha);
	Move(&mesh, vec3(-halfsize, -halfsize, -halfsize));
	return Compile(mesh, tpool);
}

class EasyLoop : public Loop
{
public:
	EasyLoop(Game* game)
		: Loop(game)
		, world2(800, 600)
	{
		const rect res = FromUpperLeftAndSize(Origo2(), direction2(world2.getWidth(), world2.getHeight()));

		boost::shared_ptr<ActorDef> crate = CreateCube(1.5f, "crate01a.jpg", &tpool, 1, true);
		boost::shared_ptr<ActorDef> glass = CreateCube(1.5f, "glass.png", &tpool, 0.5f, true);
		boost::shared_ptr<ActorDef> pattern = CreateCube(1.5f, "pattern1.png", &tpool, 1, true);

		boost::shared_ptr<World3> world( World3::Create() );
		const real spacing = 3;
		for(int i=0; i<5; ++i)
		{
			const real x = (i-2)*spacing;
			for(int i=0; i<5; ++i)
			{
				const real z = (i-2)*spacing;

				world->actor_add( Actor::Create(point3(x,-3,z), qIdentity(), crate) );
				world->actor_add( Actor::Create(point3(x,0,z), qIdentity(), glass) );
				world->actor_add( Actor::Create(point3(x,3,z), qIdentity(), pattern) );
			}
		}

		// sky texture
		world->actor_add(
			Actor::Create(Origo3(), qIdentity(),
				CreateCube(400, "_stars-texture.jpg", &tpool, 1, false)
				)
			);

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
	DemoCamera dcam;
	VfsTexturePool2 tpool;
};

int main(int, char** argv)
{
	Game game;
	InstallDefaultSystems(&game, argv[0], "entsoft", "survivors", "Survivors!");
	EasyLoop(&game).loop();
}