#include <pwn/render/fse/Target_Targets.h>
#include <pwn/render/fse/exceptions.h>
#include <pwn/render/fse/Target_BufferTarget.h>
#include <pwn/render/fse/Target_FormTarget.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			TargetPtr Targets_Create(const string& name, const core::Ptree& data)
			{
				if (name == "buffer")
				{
					return TargetPtr(new BufferTarget(data));
				}
				else if (name == "form")
				{
					return TargetPtr(new FormTarget());
				}
				else throw FseException(name + " is not a known target");
			}
		}
	}
}