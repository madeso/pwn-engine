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
#include <pwn/engine/vfstexturepool2.h>
#include <pwn/render/worldwithcameraboundobject3.h>
#include <pwn/render/fse/Pipeline.h>

#include <pwn/render/shaderpool.h>

using namespace pwn;
using namespace pwn::engine;
using namespace pwn::render;
using namespace pwn::math;
using namespace pwn::mesh;

boost::shared_ptr<ActorDef> CreateCube(real size, const string& texture, TexturePool2* tpool, real alpha, bool out)
{
	const real halfsize = size / 2;

	Builder b = CreateBox(materials::White(), halfsize * 2, halfsize * 2, halfsize * 2, out);;
	b.materials[0].setTexture_Diffuse(texture);
	b.materials[0].diffuse.alpha(alpha);
	Move(&b, vec3(-halfsize, -halfsize, -halfsize));
	return Compile(b.asMesh(), tpool);
}

class EasyLoop : public Loop
{
private:
	VfsTexturePool2 tpool;
	fse::PipelinePtr pipe;
	pwn::render::ShaderPool tempShaderPool;

	bool followcam;
	boost::shared_ptr<World3Widget > wid;
public:

	EasyLoop(Game* game)
		: Loop(game)
	{
		World3::Ptr world(new WorldWithCameraBoundObject3(Actor::Create(Origo3(), qIdentity(), CreateCube(10, "_stars-texture.jpg", &tpool, 1, false)),
		                  World3::Create()));
		world->light_setAmbient(math::Rgba(1.0f));

		boost::shared_ptr<Actor> object = Actor::Create(point3(0, 0, 15), qIdentity(), CreateCube(1, "crate01a.jpg", &tpool, 1, true));
		object->debug = true;
		world->actor_add(object);

		wid.reset(new World3Widget(Dock::Fill(), world));
		Camera cam(point3(4, 4, 4), qIdentity(), 45, 0.1f, 1000);
		cam.pipeline = fse::Pipeline::Create("fse/normal.xml", &tempShaderPool);
		wid->updateCamera(cam);
		display.widget_add(wid);
	}

	void onKey(Key::Code key, bool isDown)
	{
		if(key == Key::Escape && isDown)
		{
			stop();
		}
	}

	void onUpdate(real delta)
	{
	}

	void onRender()
	{
		defaultRender();
	}

	void onMouse(const math::vec2 movement)
	{
	}
};

int main(int, char** argv)
{
	{
		Game game;
		InstallDefaultSystems(&game,
		                      Startup(argv[0], "entsoft", "pwn-demo-basic-camera", "pwn demo: basic camera")
		                     );
		EasyLoop(&game).loop();
	}
}
