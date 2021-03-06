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

Animation
LoadAnimation(const string& file)
{
    Animation animation;
    pwn::io::Read(&animation, file);
    return animation;
}

class EasyLoop : public Loop
{
private:
    Animation walk;
    Animation def;
    boost::shared_ptr<Actor> turtle;
    real pos;
    fse::PipelinePtr simple;
    fse::PipelinePtr normal;
    boost::shared_ptr<SpotLight> light;
    bool right;
    bool defan;

public:
    pwn::render::ShaderPool tempShaderPool;

    EasyLoop(Game* game) : Loop(game), right(false), pos(0), defan(true)
    {
        World3::Ptr world(new WorldWithCameraBoundObject3(
                Actor::Create(
                        Origo3(),
                        qIdentity(),
                        CreateCube(10, "_stars-texture.jpg", &tpool, 1, false)),
                World3::Create()));
        world->light_setAmbient(math::Rgba(1.0f));

        const string c = true ? "dwarf1" : "posable_guy-posable guy skin";

        def = LoadAnimation(c + "/_default.anim");
        walk = LoadAnimation(c + "/walk.anim");
        turtle = Actor::Create(
                point3(0, 0, 0),
                qIdentity(),
                LoadMesh(c + ".mesh", &tpool));
        world->actor_add(turtle);
        //world->actor_add(Actor::Create(point3(0,0,0), qIdentity(), LoadMesh("sphere.mesh", &tpool)));

        /*world->actor_add(
			Actor::Create(point3(0,0,0), qIdentity(), LoadMesh("alien_chamber.mesh", &tpool))
			);*/

        light.reset(new SpotLight());
        light->properties.diffuse.rgb(1, 1, 1);
        light->exponent = 90;
        world->light_add(light);

        boost::shared_ptr<World3Widget> wid(
                new World3Widget(Dock::Fill(), world));

        dcam.world = wid;
        simple = fse::Pipeline::Create("fse/simple-test.xml", &tempShaderPool);
        normal = fse::Pipeline::Create("fse/normal.xml", &tempShaderPool);
        dcam.camera.pipeline = normal;

        display.widget_add(wid);
    }

    void
    onKey(Key::Code key, bool isDown)
    {
        if (key == Key::Escape && isDown)
        {
            stop();
        }
        else if (key == Key::LMB)
        {
            dcam.camera.pipeline = isDown ? simple : normal;
        }
        else if (key == Key::RMB)
        {
            right = isDown;
        }
        else if (key == Key::E && isDown)
        {
            defan = !defan;
            pos = 0;
        }
        else
        {
            dcam.onKey(key, isDown);
        }
    }

    void
    onUpdate(real delta)
    {
        Pose p;
        const Animation& a = defan ? def : walk;
        pos = pwn::math::Wrap(0, pos + delta, a.getLength());
        a.getPose(pos, &p);
        turtle->setPose(p);
        dcam.update(delta, 30.0f, 10.0f);

        if (!right)
        {
            light->position = dcam.camera.position.vec;
            light->direction = dcam.camera.rotation;
        }
    }

    void
    onRender()
    {
        defaultRender();
    }

    void
    onMouse(const math::vec2 movement)
    {
        dcam.onMouse(movement);
    }

    DemoCamera dcam;
    VfsTexturePool2 tpool;
    fse::PipelinePtr pipe;
};


// we need to link to sfml-main otherwise argv becomes null and things crash and burn
// rethink this design and get and send cwd from boost filesystem to physfs
int
main(int, char** argv)
{
    try
    {
        Game game;
        InstallDefaultSystems(
                &game,
                Startup(argv[0], "entsoft", "survivors", "Survivors!"));
        EasyLoop(&game).loop();
    }
    catch(const std::string& err)
    {
        std::cout << "Error: " << err <<"\n";
    }
    catch(const char* const err)
    {
        std::cout << "Error: " << err <<"\n";
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
    }
}
