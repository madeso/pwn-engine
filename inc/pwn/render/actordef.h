#ifndef PWN_RENDER_ACTORDEF
#define PWN_RENDER_ACTORDEF

#include <vector>
#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace mesh
	{
		class Mesh;
		class Bone;
	}

	namespace render
	{
		class Part;
		class TexturePool2;

		/** A mesh definition ready for rendering.
		 */
		class ActorDef
		{
		public:
			ActorDef();
			~ActorDef();

			typedef boost::shared_ptr<Part> PartPtr;

			std::vector<PartPtr> parts;
			std::vector<mesh::Bone> bones;
		};

		boost::shared_ptr<ActorDef> Compile(const mesh::Mesh& mesh, TexturePool2* pool);
	}
}

#endif