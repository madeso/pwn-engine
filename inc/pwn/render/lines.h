#ifndef PWN_RENDER_LINES
#define PWN_RENDER_LINES

#include <pwn/math/types.h>
#include <pwn/math/rgba.h>
#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class RenderList;
		class CompiledCamera;

		/** Lines for debugging.
		*/
		class Lines
		{
		public:
			Lines();
			static boost::shared_ptr<Lines> Create();
			void render(RenderList* rl, const CompiledCamera& camera);

			real width;
			math::Rgba color;

			struct Line
			{
				Line(const math::vec3& f, const math::vec3& t);
				math::vec3 from;
				math::vec3 to;
			};

			std::vector<Line> lines;

			void add(const math::vec3& from, const math::vec3& to);
		};
	}
}

#endif
