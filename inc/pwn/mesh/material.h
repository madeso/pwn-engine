#ifndef PWN_MESH_MATERIAL
#define PWN_MESH_MATERIAL

#include <pwn/math/rgba>
#include <pwn/string>

namespace pwn
{
	namespace mesh
	{
		class Material
		{
		public:
			Material();

			string name; // todo: only added as a temporary fix for converter - remove asap

			math::Rgba ambient; // -1 - 1
			math::Rgba diffuse; // -1 - 1
			math::Rgba specular; // -1 - 1
			math::Rgba emission; // -1 - 1
			real shininess; // 0-1

			string texture_diffuse;
		};
	}
}

#endif