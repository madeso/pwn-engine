#include <pwn/render/fse/Target_FormTarget.h>
#include <pwn/render/fse/Provider.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			FormTarget::FormTarget()
			{
			}

			FormTarget::~FormTarget()
			{
			}

			void FormTarget::apply(Provider& a, World3* world, const RenderArgs& ra)
			{
				a.doProvide(world, ra);
			}

			void FormTarget::link(Linker* usr)
			{
			}

			string FormTarget::toString() const
			{
				return Target::toString() + " targeting a form";
			}
		}
	}
}
