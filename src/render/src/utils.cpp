#include <pwn/render/utils.h>
#include <pwn/string.h>

namespace pwn
{
	namespace render
	{
		void RenderFullscreenQuad(int unknown, int width, int height)
		{
		}
	}

	string Nullstring(const string& str, const string& def)
	{
		if( str.empty()==false ) return str;
		else return def;
	}
}
