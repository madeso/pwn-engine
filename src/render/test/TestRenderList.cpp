#include <gtest/gtest.h>

#include <vector>

#include "catchy/vectorequals.h"

#include <pwn/render/renderlist.h>
#include <pwn/render/compiledmesh.h>
#include <pwn/render/material.h>
#include <pwn/core/idpool.h>
#include <pwn/math/operations.h>
#include <pwn/render/poseable.h>
#include "pwn/core/str.h"

namespace test  //SUITE(testRenderList)
{
using namespace pwn;
using namespace pwn::math;
using namespace pwn::core;
using namespace pwn::render;

const bool useGlCommands = false;
const bool useGlTexture = false;

typedef std::vector<int> List;

catchy::FalseString
ElementsAre(const List& lhs, const List& rhs)
{
    return catchy::VectorEquals
    (
        lhs,
        rhs,
        [](int i)
        {
            return (Str() << i).toString();
        },
        [](int lhsi, int rhsi)
        {
            return lhsi != rhsi ? catchy::FalseString::False("differs") : catchy::FalseString::True();
        }
    );
}

class CompiledMeshStub : public CompiledMesh
{
public:
    CompiledMeshStub(const int index, List* list) : index(index), list(list)
    {
    }
    void
    render(const mesh::CompiledPose&)
    {
        list->push_back(index);
    }
    const int index;
    List* list;
};

RenderList::MeshPtr
CreateMesh(const int index, List* list)
{
    return RenderList::MeshPtr(new CompiledMeshStub(index, list));
}

RenderList::MaterialPtr
CreateMaterial(bool trans, Material::TexturePtr texture)
{
    RenderList::MaterialPtr m(new Material());
    m->hasTransperency = trans;
    m->texture = texture;
    return m;
}

Material::TexturePtr
CreateTexture(IdPool* pool)
{
    return Material::TexturePtr(new Texture2(pool, useGlTexture));
}

namespace
{
    const mat44
    CreateTranslationMatrixFromdepth(const real z)
    {
        return cmat44(vec3(0, 0, z));
    }
}

TEST(testRenderList, CommonMeshMat)
{
    IdPool pool;
    Material::TexturePtr tNull;
    Material::TexturePtr t1 = CreateTexture(&pool);
    Material::TexturePtr t2 = CreateTexture(&pool);
    Material::TexturePtr t3 = CreateTexture(&pool);

    mat44 m1 = CreateTranslationMatrixFromdepth(0);
    mat44 m0 = CreateTranslationMatrixFromdepth(10);
    mat44 m2 = CreateTranslationMatrixFromdepth(20);
    mat44 m3 = CreateTranslationMatrixFromdepth(30);

    RenderList rl(useGlCommands);
    List list;
    Poseable poseable;


    // basic tests to check that it renders it
    SECTION("testRender_Single")
    {
        rl.begin();
        rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {0}));
    }

    SECTION("testRender_Several")
    {
        rl.begin();
        rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {0, 0, 0, 0, 0}));
    }

    // test alpha/non alpha sorting
    SECTION("testRender_Alpha_RightOrder")
    {
        rl.begin();
        rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {1, 2}));
    }

    SECTION("testRender_Alpha_WrongOrder")
    {
        rl.begin();
        rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0, &poseable);
        rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {1, 2}));
    }

    // test sort distance order...
    SECTION("testRender_Distance_Solid_RightOrder")
    {
        rl.begin();
        rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m1, &poseable);
        rl.add(CreateMesh(2, &list), CreateMaterial(false, tNull), m2, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {0, 1, 2}));
    }

    SECTION("testRender_Distance_Solid_WrongOrder")
    {
        rl.begin();
        rl.add(CreateMesh(2, &list), CreateMaterial(false, tNull), m2, &poseable);
        rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m1, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {0, 1, 2}));
    }

    SECTION("testRender_Distance_Transparent_RightOrder")
    {
        rl.begin();
        rl.add(CreateMesh(0, &list), CreateMaterial(true, tNull), m2, &poseable);
        rl.add(CreateMesh(1, &list), CreateMaterial(true, tNull), m1, &poseable);
        rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {0, 1, 2}));
    }

    SECTION("testRender_Distance_Transparent_WrongOrder")
    {
        rl.begin();
        rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0, &poseable);
        rl.add(CreateMesh(0, &list), CreateMaterial(true, tNull), m2, &poseable);
        rl.add(CreateMesh(1, &list), CreateMaterial(true, tNull), m1, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {0, 1, 2}));
    }

    // test sort textures order...
    SECTION("testRender_Texture_RightOrder")
    {
        rl.begin();
        rl.add(CreateMesh(0, &list), CreateMaterial(false, t1), m0, &poseable);
        rl.add(CreateMesh(1, &list), CreateMaterial(false, t2), m0, &poseable);
        rl.add(CreateMesh(2, &list), CreateMaterial(false, t3), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {0, 1, 2}));
    }

    SECTION("testRender_Texture_WrongOrder")
    {
        rl.begin();
        rl.add(CreateMesh(2, &list), CreateMaterial(false, t3), m0, &poseable);
        rl.add(CreateMesh(0, &list), CreateMaterial(false, t1), m0, &poseable);
        rl.add(CreateMesh(1, &list), CreateMaterial(false, t2), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {0, 1, 2}));
    }

    // test complex sorting order
    SECTION("testRender_Complex_RightOrder")
    {
        rl.begin();
        rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.add(CreateMesh(2, &list), CreateMaterial(false, tNull), m1, &poseable);
        rl.add(CreateMesh(3, &list), CreateMaterial(false, t1), m0, &poseable);
        rl.add(CreateMesh(4, &list), CreateMaterial(false, t1), m1, &poseable);
        rl.add(CreateMesh(5, &list), CreateMaterial(false, t2), m0, &poseable);
        rl.add(CreateMesh(6, &list), CreateMaterial(false, t2), m1, &poseable);
        rl.add(CreateMesh(7, &list), CreateMaterial(false, t3), m0, &poseable);
        rl.add(CreateMesh(8, &list), CreateMaterial(false, t3), m1, &poseable);

        rl.add(CreateMesh(9, &list), CreateMaterial(true, tNull), m1, &poseable);
        rl.add(CreateMesh(10, &list), CreateMaterial(true, t1), m1, &poseable);
        rl.add(CreateMesh(11, &list), CreateMaterial(true, t2), m1, &poseable);
        rl.add(CreateMesh(12, &list), CreateMaterial(true, t3), m1, &poseable);
        rl.add(CreateMesh(13, &list), CreateMaterial(true, tNull), m0, &poseable);
        rl.add(CreateMesh(14, &list), CreateMaterial(true, t1), m0, &poseable);
        rl.add(CreateMesh(15, &list), CreateMaterial(true, t2), m0, &poseable);
        rl.add(CreateMesh(16, &list), CreateMaterial(true, t3), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}));
    }

    SECTION("testRender_Complex_WrongOrder")
    {
        rl.begin();
        rl.add(CreateMesh(7, &list), CreateMaterial(false, t3), m0, &poseable);
        rl.add(CreateMesh(8, &list), CreateMaterial(false, t3), m1, &poseable);
        rl.add(CreateMesh(16, &list), CreateMaterial(true, t3), m0, &poseable);
        rl.add(CreateMesh(3, &list), CreateMaterial(false, t1), m0, &poseable);
        rl.add(CreateMesh(6, &list), CreateMaterial(false, t2), m1, &poseable);
        rl.add(CreateMesh(14, &list), CreateMaterial(true, t1), m0, &poseable);
        rl.add(CreateMesh(15, &list), CreateMaterial(true, t2), m0, &poseable);
        rl.add(CreateMesh(13, &list), CreateMaterial(true, tNull), m0, &poseable);
        rl.add(CreateMesh(11, &list), CreateMaterial(true, t2), m1, &poseable);
        rl.add(CreateMesh(2, &list), CreateMaterial(false, tNull), m1, &poseable);
        rl.add(CreateMesh(5, &list), CreateMaterial(false, t2), m0, &poseable);
        rl.add(CreateMesh(12, &list), CreateMaterial(true, t3), m1, &poseable);
        rl.add(CreateMesh(9, &list), CreateMaterial(true, tNull), m1, &poseable);
        rl.add(CreateMesh(10, &list), CreateMaterial(true, t1), m1, &poseable);
        rl.add(CreateMesh(4, &list), CreateMaterial(false, t1), m1, &poseable);
        rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m0, &poseable);
        rl.end(false);

        CHECK(ElementsAre(list, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}));
    }
}

}
