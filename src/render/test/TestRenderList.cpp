#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <pwn/render/renderlist.h>
#include <pwn/render/compiledmesh.h>
#include <pwn/render/material.h>
#include <pwn/core/idpool.h>
#include <pwn/math/operations.h>
#include <pwn/render/poseable.h>

namespace test //SUITE(testRenderList)
{
	using namespace pwn;
	using namespace pwn::math;
	using namespace pwn::core;
	using namespace pwn::render;

	const bool useGlCommands = false;
	const bool useGlTexture = false;

	typedef std::vector<int> List;

	class CompiledMeshStub
		: public CompiledMesh
	{
	public:
		CompiledMeshStub(const int index, List* list)
			: index(index)
			, list(list)
		{
		}
		void render(const mesh::CompiledPose&)
		{
			list->push_back(index);
		}
		const int index;
		List* list;
	};

	RenderList::MeshPtr CreateMesh(const int index, List* list)
	{
		return RenderList::MeshPtr(new CompiledMeshStub(index, list));
	}

	RenderList::MaterialPtr CreateMaterial(bool trans, Material::TexturePtr texture)
	{
		RenderList::MaterialPtr m(new Material());
		m->hasTransperency = trans;
		m->texture = texture;
		return m;
	}

	Material::TexturePtr CreateTexture(IdPool* pool)
	{
		return Material::TexturePtr(new Texture2(pool, useGlTexture));
	}

	namespace
	{
		const mat44 CreateTranslationMatrixFromdepth(const real z)
		{
			return cmat44(vec3(0, 0, z));
		}
	}

	struct CommonMeshMat : ::testing::Test
	{
		CommonMeshMat()
			: m0(CreateTranslationMatrixFromdepth(0))
			, m1(CreateTranslationMatrixFromdepth(10))
			, m2(CreateTranslationMatrixFromdepth(20))
			, m3(CreateTranslationMatrixFromdepth(30))

			, rl(useGlCommands)
		{
			t1 = CreateTexture(&pool);
			t2 = CreateTexture(&pool);
			t3 = CreateTexture(&pool);
		}

		IdPool pool;
		Material::TexturePtr tNull;
		Material::TexturePtr t1;
		Material::TexturePtr t2;
		Material::TexturePtr t3;

		mat44 m0;
		mat44 m1;
		mat44 m2;
		mat44 m3;

		RenderList rl;
		List list;
		Poseable poseable;
	};

	// basic tests to check that it renders it
	TEST_F(CommonMeshMat, testRender_Single)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(0));
	}

	TEST_F(CommonMeshMat, testRender_Several)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(0, 0, 0, 0, 0));
	}

	// test alpha/non alpha sorting
	TEST_F(CommonMeshMat, testRender_Alpha_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(1, 2));
	}

	TEST_F(CommonMeshMat, testRender_Alpha_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0, &poseable);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(1, 2));
	}

	// test sort distance order...
	TEST_F(CommonMeshMat, testRender_Distance_Solid_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m1, &poseable);
		rl.add(CreateMesh(2, &list), CreateMaterial(false, tNull), m2, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(0, 1, 2));
	}

	TEST_F(CommonMeshMat, testRender_Distance_Solid_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(2, &list), CreateMaterial(false, tNull), m2, &poseable);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m1, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(0, 1, 2));
	}

	TEST_F(CommonMeshMat, testRender_Distance_Transparent_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(true, tNull), m2, &poseable);
		rl.add(CreateMesh(1, &list), CreateMaterial(true, tNull), m1, &poseable);
		rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(0, 1, 2));
	}

	TEST_F(CommonMeshMat, testRender_Distance_Transparent_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0, &poseable);
		rl.add(CreateMesh(0, &list), CreateMaterial(true, tNull), m2, &poseable);
		rl.add(CreateMesh(1, &list), CreateMaterial(true, tNull), m1, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(0, 1, 2));
	}

	// test sort textures order...
	TEST_F(CommonMeshMat, testRender_Texture_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(false, t1), m0, &poseable);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, t2), m0, &poseable);
		rl.add(CreateMesh(2, &list), CreateMaterial(false, t3), m0, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(0, 1, 2));
	}

	TEST_F(CommonMeshMat, testRender_Texture_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(2, &list), CreateMaterial(false, t3), m0, &poseable);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, t1), m0, &poseable);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, t2), m0, &poseable);
		rl.end(false);

		ASSERT_THAT(list, testing::ElementsAre(0, 1, 2));
	}

	// test complex sorting order
	TEST_F(CommonMeshMat, testRender_Complex_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(1,  &list), CreateMaterial(false, tNull), m0, &poseable);
		rl.add(CreateMesh(2,  &list), CreateMaterial(false, tNull), m1, &poseable);
		rl.add(CreateMesh(3,  &list), CreateMaterial(false, t1),    m0, &poseable);
		rl.add(CreateMesh(4,  &list), CreateMaterial(false, t1),    m1, &poseable);
		rl.add(CreateMesh(5,  &list), CreateMaterial(false, t2),    m0, &poseable);
		rl.add(CreateMesh(6,  &list), CreateMaterial(false, t2),    m1, &poseable);
		rl.add(CreateMesh(7,  &list), CreateMaterial(false, t3),    m0, &poseable);
		rl.add(CreateMesh(8,  &list), CreateMaterial(false, t3),    m1, &poseable);

		rl.add(CreateMesh(9,  &list), CreateMaterial(true, tNull), m1, &poseable);
		rl.add(CreateMesh(10, &list), CreateMaterial(true, t1),    m1, &poseable);
		rl.add(CreateMesh(11, &list), CreateMaterial(true, t2),    m1, &poseable);
		rl.add(CreateMesh(12, &list), CreateMaterial(true, t3),    m1, &poseable);
		rl.add(CreateMesh(13, &list), CreateMaterial(true, tNull), m0, &poseable);
		rl.add(CreateMesh(14, &list), CreateMaterial(true, t1),    m0, &poseable);
		rl.add(CreateMesh(15, &list), CreateMaterial(true, t2),    m0, &poseable);
		rl.add(CreateMesh(16, &list), CreateMaterial(true, t3),    m0, &poseable);
		rl.end(false);

		int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
		ASSERT_THAT(list, testing::ElementsAreArray(arr));
	}

	TEST_F(CommonMeshMat, testRender_Complex_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(7,  &list), CreateMaterial(false, t3),    m0, &poseable);
		rl.add(CreateMesh(8,  &list), CreateMaterial(false, t3),    m1, &poseable);
		rl.add(CreateMesh(16, &list), CreateMaterial(true,  t3),    m0, &poseable);
		rl.add(CreateMesh(3,  &list), CreateMaterial(false, t1),    m0, &poseable);
		rl.add(CreateMesh(6,  &list), CreateMaterial(false, t2),    m1, &poseable);
		rl.add(CreateMesh(14, &list), CreateMaterial(true,  t1),    m0, &poseable);
		rl.add(CreateMesh(15, &list), CreateMaterial(true,  t2),    m0, &poseable);
		rl.add(CreateMesh(13, &list), CreateMaterial(true,  tNull), m0, &poseable);
		rl.add(CreateMesh(11, &list), CreateMaterial(true,  t2),    m1, &poseable);
		rl.add(CreateMesh(2,  &list), CreateMaterial(false, tNull), m1, &poseable);
		rl.add(CreateMesh(5,  &list), CreateMaterial(false, t2),    m0, &poseable);
		rl.add(CreateMesh(12, &list), CreateMaterial(true,  t3),    m1, &poseable);
		rl.add(CreateMesh(9,  &list), CreateMaterial(true,  tNull), m1, &poseable);
		rl.add(CreateMesh(10, &list), CreateMaterial(true,  t1),    m1, &poseable);
		rl.add(CreateMesh(4,  &list), CreateMaterial(false, t1),    m1, &poseable);
		rl.add(CreateMesh(1,  &list), CreateMaterial(false, tNull), m0, &poseable);

		rl.end(false);

		int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
		ASSERT_THAT(list, testing::ElementsAreArray(arr));
	}
}
