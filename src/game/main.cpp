#include <pwn/engine/loop.h>
#include <pwn/engine/game.h>
#include <pwn/engine/startup.h>

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
#include <pwn/io/io.h>
#include <pwn/render/worldwithcameraboundobject3.h>
#include <pwn/render/fse/pipeline.h>

using namespace pwn;
using namespace pwn::engine;
using namespace pwn::render;
using namespace pwn::math;
using namespace pwn::mesh;

boost::shared_ptr<ActorDef> CreateCube(real size, const string& texture, TexturePool2* tpool, real alpha, bool out)
{
	Mesh mesh;
	const real halfsize = size/2;

	Builder b;
	b.setBox(materials::White(), halfsize*2, halfsize*2, halfsize*2, out);
	b.makeMesh(mesh, 0);
	mesh.materials[0].setTexture_Diffuse(texture);
	mesh.materials[0].diffuse.alpha(alpha);
	Move(&mesh, vec3(-halfsize, -halfsize, -halfsize));
	return Compile(mesh, tpool);
}

boost::shared_ptr<ActorDef> LoadMesh(const string& file, TexturePool2* tpool)
{
	Mesh mesh;
	pwn::io::Read(&mesh, file);
	return Compile(mesh, tpool);
}

Animation LoadAnimation(const string& file)
{
	Animation animation;
	pwn::io::Read(&animation, file);
	return animation;
}

class EasyLoop : public Loop
{
private:
	Animation walk;
	boost::shared_ptr<Actor> turtle;
	real pos;
public:
	EasyLoop(Game* game)
		: Loop(game)
	{
		World3::Ptr world( new WorldWithCameraBoundObject3(Actor::Create(Origo3(), qIdentity(), CreateCube(10, "_stars-texture.jpg", &tpool, 1, false) ),
			World3::Create()) );

		//world->actor_add( Actor::Create(point3(0,0,0), qIdentity(), LoadMesh("fighter1.mesh", &tpool)) );

		walk = LoadAnimation("dwarf1/Walk.anim");
		turtle = Actor::Create(point3(0,0,0), qIdentity(), LoadMesh("dwarf1.mesh", &tpool));
		world->actor_add(turtle);

		boost::shared_ptr<World3Widget > wid( new World3Widget( Dock::Fill(), world ) );

		dcam.world = wid;
		dcam.camera.pipeline = fse::Pipeline::Create("fse/normal.xml");
		//dcam.camera.position = point3(-20,20,-40);
		//dcam.camera.orientation = qLookAt(dcam.camera.position.vec, vec3(0,0,0), Up());

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
		Pose p;
		pos = pwn::math::Wrap(0, pos+delta, walk.getLength());
		walk.getPose(pos, &p);
		turtle->setPose(p);
		dcam.update(delta, 3.0f, 10.0f);
	}

	void onRender()
	{
		defaultRender();
	}

	void onMouse(const math::vec2 movement)
	{
		dcam.onMouse(movement);
	}

	DemoCamera dcam;
	VfsTexturePool2 tpool;
	fse::PipelinePtr pipe;
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