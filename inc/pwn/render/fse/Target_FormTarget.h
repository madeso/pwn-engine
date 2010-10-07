#ifndef PWN_RENDER_FSE_TARGET_FORMTARGET
#define PWN_RENDER_FSE_TARGET_FORMTARGET

#include <pwn/render/fse/Target.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class FormTarget
				: public Target
			{
			public:
				FormTarget(int width, int height);
				~FormTarget();

				void apply(Provider& a, World3* world, const RenderArgs& ra);
				int getWidth();
				int getHeight();
				void link(Linker* usr);
				string toString() const;
				
			private:
				int width;
				int height;
			};
		}
	}
}

#endif
