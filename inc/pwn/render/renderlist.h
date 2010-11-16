#ifndef PWN_RENDER_RENDERLIST
#define PWN_RENDER_RENDERLIST

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <pwn/math/types.h>
#include <vector>

namespace pwn
{
	namespace render
	{
		class CompiledMesh;
		class Material;
		class Texture2;
		class Poseable;

		/// sorts render calls, and might utilize frame-to-frame coherence later on
		class RenderList
			: boost::noncopyable
		{
		public:
			RenderList(bool useGlCommands);
			~RenderList();

			typedef boost::shared_ptr<CompiledMesh> MeshPtr;
			typedef boost::shared_ptr<Material> MaterialPtr;

			typedef uint64 ID;

			/// simple render command, might include other commands later on
			struct Command
			{
				Command();
				Command(MeshPtr mesh, MaterialPtr material, const math::mat44& mat, Poseable* pos);

				MaterialPtr material;
				MeshPtr mesh;
				math::mat44 mat;
				Poseable* poseable;

				ID id;
			};

			void begin();
			void add(MeshPtr mesh, MaterialPtr material, const math::mat44& mat, Poseable* pos);
			void end(bool applyMaterials);

			typedef std::vector<Command> CommandList;
			void apply(MaterialPtr material, bool applyMaterials);

		private:
			void render(const CommandList& commands, bool applyMaterials);
			const bool useGlCommands;
			CommandList transparent;
			CommandList solid;

			Texture2* texture;
			bool applied;
		};
	}
}

#endif