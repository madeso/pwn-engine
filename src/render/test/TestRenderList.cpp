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

	struct CommonMeshMat
	{
		CommonMeshMat()
			: m0( mat44Identity() )
			, m1( mat44Identity() )
			, m2( mat44Identity() )
			, m3( mat44Identity() )
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
	};

	TEST_FIXTURE(CommonMeshMat, testRenderIt)
	{
		RenderList rl(useGlCommands);
		List list;

		const int myMesh = 0;

		rl.begin();
		rl.add(CreateMesh(myMesh, &list), CreateMaterial(false, tNull), m0);
		rl.end();

		const std::size_t expectedSize = 1;
		CHECK_EQUAL(expectedSize, list.size() );
		if( list.size() != expectedSize ) return;
		CHECK_EQUAL(myMesh, list[0]);
	}
}