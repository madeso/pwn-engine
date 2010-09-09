#include <pwn/meshio/io.h>

#include <pwn/mesh/mesh.h>
#include <pwn/math/operations.h>
#include "vfs.h"
#include <boost/foreach.hpp>
#include <map>

namespace pwn
{
	namespace meshio
	{
		const pwn::uint8 kVersion = 1;

		template<class AnimationArg>
		class AnimationFile
		{
		public:
			template <typename VersionType>
			static void handle(VirtualFile& vf, AnimationArg animation, VersionType version)
			{
				vf.handle8(version);
				if( version != kVersion ) throw "animation version mismatch";
				vf.handleReal(animation.length);

				pwn::uint32 size = HandleVectorSize(vf, animation.bones);
				for(pwn::uint32 i=0;i<size; ++i)
				{
					HandleVector(vf, animation.bones[i].fp);
					HandleVector(vf, animation.bones[i].fr);
				}
			}
		};

		void Write(const mesh::Animation& animation, const pwn::string& filename)
		{
			VirtualFile vf(filename, false);
			AnimationFile<const mesh::Animation&>::handle<const pwn::uint8>(vf, animation, kVersion);
		}

		void Read(mesh::Animation* animation, const pwn::string& filename)
		{
			pwn::uint8 version = kVersion;
			VirtualFile vf(filename, true);
			AnimationFile<mesh::Animation&>::handle<pwn::uint8&>(vf, *animation, version);
		}
	}
}