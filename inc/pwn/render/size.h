#ifndef PWN_RENDER_SIZE
#define PWN_RENDER_SIZE

namespace pwn
{
	namespace render
	{
		/** A 2 dimensional size.
		 for example used when stroing different fbo sizes in a map.
		 */
		struct Size
		{
			Size();
			Size(int w, int h);

			int width;
			int height;

			bool operator<(const Size& o) const;
		};
	}
}

#endif
