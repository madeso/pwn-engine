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
			void end();

			typedef std::vector<Command> CommandList;
			void apply(MaterialPtr material);

		private:
			void render(const CommandList& commands);
			const bool useGlCommands;
			CommandList transparent;
			CommandList solid;

			Texture2* texture;
			bool applied;
		};
	}
}

#endif