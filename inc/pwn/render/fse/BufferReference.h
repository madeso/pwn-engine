#ifndef PWN_RENDER_FSE_BUFFERREFERENCE
#define PWN_RENDER_FSE_BUFFERREFERENCE

#include <pwn/string.h>
#include <boost/utility.hpp>
#include <pwn/render/fbo.h>
#include <pwn/render/fse/types.h>
namespace pwn
{
	namespace render
	{
		class RenderArgs;

		namespace fse
		{
			class BufferReference
				: boost::noncopyable
			{
			public:
				BufferReference(const string& name);
				const string& getName() const;
				~BufferReference();
				
			public:
				void setBuffer(FboPtr fbo);
				void bindTexture(int location);
				void updateTexture(Provider& a, World3* world, const RenderArgs& ra); // calls doProvide
				int getWidth() const;
				int getHeight() const;
				
			private:
				FboPtr buffer;
				const string name;
			};
		}
	}
}

#endif
