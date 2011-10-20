#include <gtest/gtest.h>

#include <pwn/mesh/mesh.h>
#include <pwn/mesh/builder.h>
#include <pwn/mesh/predefinedmaterials.h>
#include <pwn/math/types.h>

#define GTEST(x) TEST(Mesh, x)

namespace test
{
	using namespace pwn;
	using namespace pwn::mesh;
	using namespace pwn::math;

	/*GTEST(BasicConstruction)
	{
		Mesh m;
	}*/

	GTEST(AdvancedExample)
	{
		const real size = 10;
		const real halfsize = size / 2;
		bool out = false;

		Builder b = CreateBox(materials::White(), halfsize * 2, halfsize * 2, halfsize * 2, out);
		b.materials[0].setTexture_Diffuse("hello");
		b.materials[0].diffuse.alpha(0.25f);
		Move(&b, vec3(-halfsize, -halfsize, -halfsize));
		Mesh m = b.asMesh();
		//return Compile(mesh, tpool);
	}
}
