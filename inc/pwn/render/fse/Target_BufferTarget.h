#ifndef PWN_RENDER_FSE_TARGET_BUFFERTARGET
#define PWN_RENDER_FSE_TARGET_BUFFERTARGET

#include <pwn/render/fse/Target.h>
#include <boost/noncopyable.hpp>
#include <pwn/core/config.h>

namespace pwn
{
	namespace render
	{
		class RenderArgs;

		namespace fse
		{
			class BufferTarget
				: public Target
				, boost::noncopyable
			{
			public:
				BufferTarget(const core::Ptree& el);
				~BufferTarget();

				void apply(Provider& a, World3* world, const RenderArgs& ra); // Calls doProvide
				void link(Linker* usr);
				string toString() const;

			private:
				BufferReferencePtr buffer;
				const string getName() const;

				const int width;
				const int height;
			};
		}
	}
}

#endif
