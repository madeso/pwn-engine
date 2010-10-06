#include <pwn/render/size.h>

namespace pwn
{
	namespace render
	{
		Size::Size()
			: width(0)
			, height(0)
		{
		}

		Size::Size(int w, int h)
			: width(w)
			, height(h)
		{
		}

		bool Size::operator<(const Size& o) const
		{
			if( width != o.width ) return width < o.width;
			else return height < o.height;
		}
	}
}
