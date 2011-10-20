#ifndef PWN_RENDER_RENDERLIST
#define PWN_RENDER_RENDERLIST

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <pwn/math/types.h>
#include <pwn/math/rgba.h>
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

			enum DebugRenderType
			{
			   kUndefined, kDebugRenderPoints, kDebugRenderLines, kDebugRenderMatrix
			};

			struct DebugCommand
			{
				DebugCommand();
				DebugCommand(const math::mat44& mat, Poseable* pos, DebugRenderType dc);
				math::mat44 mat;
				Poseable* poseable;
				DebugRenderType type;
			};

			struct LineCommand
			{
				LineCommand();
				LineCommand(real width, const math::Rgba& color, const math::vec3& from, const math::vec3& to);

				real width;
				math::Rgba color;
				math::vec3 from;
				math::vec3 to;
			};

			void begin();
			void add(MeshPtr mesh, MaterialPtr material, const math::mat44& mat, Poseable* pos);
			void add(const math::mat44& mat, Poseable* pos);
			void add(const math::mat44& mat);
			void end(bool applyMaterials);
			void add(real width, const math::Rgba& color, const math::vec3& from, const math::vec3& to);

			typedef std::vector<Command> CommandList;
			typedef std::vector<DebugCommand> DebugCommandList;
			typedef std::vector<LineCommand> LineCommandList;
			void apply(MaterialPtr material, bool applyMaterials);

		private:
			void render_debug();
			void render_lines();
			void render(const CommandList& commands, bool applyMaterials);
			const bool useGlCommands;
			CommandList transparent;
			CommandList solid;
			DebugCommandList debug;
			LineCommandList lines;

			Texture2* texture;
			bool applied;
		};
	}
}

#endif
