#include <UnitTest++.h>

#include <vector>
#include <pwn/render/renderlist>
#include <pwn/render/compiledmesh>
#include <pwn/render/material>
#include <pwn/core/idpool>
#include <pwn/math/operations>

SUITE(testRenderList)
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
		void render()
		{
			list->push_back(index);
		}
		const int index;
		List* list;
	};

	RenderList::MeshPtr CreateMesh(const int index, List* list)
	{
		return RenderList::MeshPtr( new CompiledMeshStub(index, list) );
	}

	RenderList::MaterialPtr CreateMaterial(bool trans, Material::TexturePtr texture)
	{
		RenderList::MaterialPtr m( new Material() );
		m->hasTransperency = trans;
		m->texture = texture;
		return m;
	}

	Material::TexturePtr CreateTexture(IdPool* pool)
	{
		return Material::TexturePtr( new Texture2(pool, useGlTexture) );
	}

	namespace
	{
		const mat44 CreateTranslationMatrixFromdepth(const real z)
		{
			return cmat44(vec3(0,0,z) );
		}
	}


#define NOPAREN(...) __VA_ARGS__ 

#define TEST_VECTOR(argList, argSize, argValues)  \
	const std::size_t expectedSize = argSize;      \
	CHECK_EQUAL(expectedSize, argList.size() );     \
	if( argList.size() != expectedSize ) return;   \
	int order[argSize] = { NOPAREN argValues };     \
	CHECK_ARRAY_EQUAL(order, argList, expectedSize);

	struct CommonMeshMat
	{
		CommonMeshMat()
			: m0( CreateTranslationMatrixFromdepth(0) )
			, m1( CreateTranslationMatrixFromdepth(10) )
			, m2( CreateTranslationMatrixFromdepth(20) )
			, m3( CreateTranslationMatrixFromdepth(30) )

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
	};

	// basic tests to check that it renders it
	TEST_FIXTURE(CommonMeshMat, testRender_Single)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0);
		rl.end();

		TEST_VECTOR(list, 1, (0) );
	}
	TEST_FIXTURE(CommonMeshMat, testRender_Several)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0);
		rl.end();

		TEST_VECTOR(list, 5, (0, 0, 0, 0, 0) );
	}

	// test alpha/non alpha sorting
	TEST_FIXTURE(CommonMeshMat, testRender_Alpha_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m0);
		rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0);
		rl.end();

		TEST_VECTOR(list, 2, (1, 2) );
	}
	TEST_FIXTURE(CommonMeshMat, testRender_Alpha_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m0);
		rl.end();

		TEST_VECTOR(list, 2, (1, 2) );
	}

	// test sort distance order...
	TEST_FIXTURE(CommonMeshMat, testRender_Distance_Solid_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m1);
		rl.add(CreateMesh(2, &list), CreateMaterial(false, tNull), m2);
		rl.end();

		TEST_VECTOR(list, 3, (0, 1, 2) );
	}
	TEST_FIXTURE(CommonMeshMat, testRender_Distance_Solid_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(2, &list), CreateMaterial(false, tNull), m2);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, tNull), m0);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, tNull), m1);
		rl.end();

		TEST_VECTOR(list, 3, (0, 1, 2) );
	}
	TEST_FIXTURE(CommonMeshMat, testRender_Distance_Transparent_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(true, tNull), m2);
		rl.add(CreateMesh(1, &list), CreateMaterial(true, tNull), m1);
		rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0);
		rl.end();

		TEST_VECTOR(list, 3, (0, 1, 2) );
	}
	TEST_FIXTURE(CommonMeshMat, testRender_Distance_Transparent_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(2, &list), CreateMaterial(true, tNull), m0);
		rl.add(CreateMesh(0, &list), CreateMaterial(true, tNull), m2);
		rl.add(CreateMesh(1, &list), CreateMaterial(true, tNull), m1);
		rl.end();

		TEST_VECTOR(list, 3, (0, 1, 2) );
	}

	// test sort textures order...
	TEST_FIXTURE(CommonMeshMat, testRender_Texture_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(0, &list), CreateMaterial(false, t1), m0);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, t2), m0);
		rl.add(CreateMesh(2, &list), CreateMaterial(false, t3), m0);
		rl.end();

		TEST_VECTOR(list, 3, (0, 1, 2 ) );
	}
	TEST_FIXTURE(CommonMeshMat, testRender_Texture_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(2, &list), CreateMaterial(false, t3), m0);
		rl.add(CreateMesh(0, &list), CreateMaterial(false, t1), m0);
		rl.add(CreateMesh(1, &list), CreateMaterial(false, t2), m0);
		rl.end();

		TEST_VECTOR(list, 3, (0, 1, 2 ) );
	}

	// test complex sorting order
	TEST_FIXTURE(CommonMeshMat, testRender_Complex_RightOrder)
	{
		rl.begin();
		rl.add(CreateMesh(1,  &list), CreateMaterial(false, tNull), m0);
		rl.add(CreateMesh(2,  &list), CreateMaterial(false, tNull), m1);
		rl.add(CreateMesh(3,  &list), CreateMaterial(false, t1),    m0);
		rl.add(CreateMesh(4,  &list), CreateMaterial(false, t1),    m1);
		rl.add(CreateMesh(5,  &list), CreateMaterial(false, t2),    m0);
		rl.add(CreateMesh(6,  &list), CreateMaterial(false, t2),    m1);
		rl.add(CreateMesh(7,  &list), CreateMaterial(false, t3),    m0);
		rl.add(CreateMesh(8,  &list), CreateMaterial(false, t3),    m1);

		rl.add(CreateMesh(9,  &list), CreateMaterial(true, tNull), m1);
		rl.add(CreateMesh(10, &list), CreateMaterial(true, t1),    m1);
		rl.add(CreateMesh(11, &list), CreateMaterial(true, t2),    m1);
		rl.add(CreateMesh(12, &list), CreateMaterial(true, t3),    m1);
		rl.add(CreateMesh(13, &list), CreateMaterial(true, tNull), m0);
		rl.add(CreateMesh(14, &list), CreateMaterial(true, t1),    m0);
		rl.add(CreateMesh(15, &list), CreateMaterial(true, t2),    m0);
		rl.add(CreateMesh(16, &list), CreateMaterial(true, t3),    m0);
		rl.end();

		TEST_VECTOR(list, 16, (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16) );
	}
	TEST_FIXTURE(CommonMeshMat, testRender_Complex_WrongOrder)
	{
		rl.begin();
		rl.add(CreateMesh(7,  &list), CreateMaterial(false, t3),    m0);
		rl.add(CreateMesh(8,  &list), CreateMaterial(false, t3),    m1);
		rl.add(CreateMesh(16, &list), CreateMaterial(true, t3),    m0);
		rl.add(CreateMesh(3,  &list), CreateMaterial(false, t1),    m0);
		rl.add(CreateMesh(6,  &list), CreateMaterial(false, t2),    m1);
		rl.add(CreateMesh(14, &list), CreateMaterial(true, t1),    m0);
		rl.add(CreateMesh(15, &list), CreateMaterial(true, t2),    m0);
		rl.add(CreateMesh(13, &list), CreateMaterial(true, tNull), m0);
		rl.add(CreateMesh(11, &list), CreateMaterial(true, t2),    m1);
		rl.add(CreateMesh(2,  &list), CreateMaterial(false, tNull), m1);
		rl.add(CreateMesh(5,  &list), CreateMaterial(false, t2),    m0);
		rl.add(CreateMesh(12, &list), CreateMaterial(true, t3),    m1);
		rl.add(CreateMesh(9,  &list), CreateMaterial(true, tNull), m1);
		rl.add(CreateMesh(10, &list), CreateMaterial(true, t1),    m1);
		rl.add(CreateMesh(4,  &list), CreateMaterial(false, t1),    m1);
		rl.add(CreateMesh(1,  &list), CreateMaterial(false, tNull), m0);
		
		rl.end();

		TEST_VECTOR(list, 16, (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16) );
	}
}