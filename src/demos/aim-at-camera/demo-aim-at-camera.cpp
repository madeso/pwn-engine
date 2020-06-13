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
#include <pwn/render/fse/Pipeline.h>
#include <pwn/render/light.h>
#include <pwn/engine/demomovement.h>
#include <pwn/render/utils.h>

#include <pwn/render/shaderpool.h>

using namespace pwn;
using namespace pwn::engine;
using namespace pwn::render;
using namespace pwn::math;
using namespace pwn::mesh;

boost::shared_ptr<ActorDef>
CreateCube(
        real size,
        const string& texture,
        TexturePool2* tpool,
        real alpha,
        bool out)
{
    const real halfsize = size / 2;

    Builder b = CreateBox(
            materials::White(),
            halfsize * 2,
            halfsize * 2,
            halfsize * 2,
            out);
    ;
    b.materials[0].setTexture_Diffuse(texture);
    b.materials[0].diffuse.alpha(alpha);
    Move(&b, vec3(-halfsize, -halfsize, -halfsize));
    return Compile(b.asMesh(), tpool);
}

boost::shared_ptr<ActorDef>
LoadMesh(const string& file, TexturePool2* tpool)
{
    Mesh mesh;
    pwn::io::Read(&mesh, file);
    return Compile(mesh, tpool);
}

class EasyLoop : public Loop
{
private:
    boost::shared_ptr<Actor> object;
    real pos;

    Camera cam;
    VfsTexturePool2 tpool;
    fse::PipelinePtr pipe;
    pwn::render::ShaderPool tempShaderPool;

    boost::shared_ptr<World3Widget> wid;
    DemoMovement ctrl;

public:
    EasyLoop(Game* game)
        : Loop(game)
        , pos(0)
        , cam(point3(10, 10, 10), qIdentity(), 45, 0.1f, 1000)
    {
        World3::Ptr world(new WorldWithCameraBoundObject3(
                Actor::Create(
                        Origo3(),
                        qIdentity(),
                        CreateCube(10, "gray.png", &tpool, 1, false)),
                World3::Create()));
        world->light_setAmbient(math::Rgba(1.0f));
        SetupGrid(world, 10, 1, 3, 2, 100);

        object = Actor::Create(
                point3(0, 0, 15),
                qIdentity(),
                CreateCube(1, "crate01a.jpg", &tpool, 1, true));
        object->debug = true;
        world->actor_add(object);

        wid.reset(new World3Widget(Dock::Fill(), world));
        cam.pipeline = fse::Pipeline::Create("fse/normal.xml", &tempShaderPool);
        display.widget_add(wid);

        //ctrl.localUp = true;
    }

    void
    onKey(Key::Code key, bool isDown)
    {
        if (key == Key::Escape && isDown)
        {
            stop();
        }
        else
        {
            ctrl.onKey(key, isDown);
        }
    }

    void
    onUpdate(real delta)
    {
        ctrl.update(&object->position, object->rotation, delta, 10.0f);
        cam.lookAt(object->position);
        object->rotation = qLookInDirection(
                Up(),
                Right());  // qLookAt(cam.position.vec, object->position.vec, Up());
    }

    void
    onRender()
    {
        wid->updateCamera(cam);
        defaultRender();
    }

    void
    onMouse(const math::vec2 movement)
    {
    }
};

int
main(int, char** argv)
{
    {
        Game game;
        InstallDefaultSystems(
                &game,
                Startup(argv[0],
                        "entsoft",
                        "pwn-demo-basic-camera",
                        "pwn demo: basic camera"));
        EasyLoop(&game).loop();
    }
}
