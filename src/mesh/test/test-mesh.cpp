#include <gtest/gtest.h>

#include <pwn/mesh/mesh.h>

#define GTEST(x) TEST(Mesh, x)

namespace test
{
	using namespace pwn::mesh;

	GTEST(BasicConstruction)
	{
		Mesh m;
	}

	GTEST(ClearOnEmpty)
	{
		Mesh m;
		m.clear();
	}
}