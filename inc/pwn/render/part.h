#ifndef PWN_RENDER_PART
#define PWN_RENDER_PART

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class CompiledMesh;
		class Material;

		class Part
		{
		public:
			Part();
			~Part();

			boost::shared_ptr<Material> material;
			boost::shared_ptr<CompiledMesh> mesh;
		};
	}
}

#endif