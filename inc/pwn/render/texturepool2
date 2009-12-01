#ifndef PWN_RENDER_TEXTUREPOOL2
#define PWN_RENDER_TEXTUREPOOL2

#include <pwn/string>
#include <pwn/core/idpool>

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace pwn
{
	namespace render
	{
		class Texture2;

		class TexturePool2
		{
		public:
			TexturePool2();
			virtual ~TexturePool2();

			boost::shared_ptr<Texture2> get(const string& path);
			// todo: add a clear function
		protected:
			virtual boost::shared_ptr<Texture2> doLoad(core::IdPool* pool, const string&) = 0;
		private:
			typedef std::map<pwn::string, boost::weak_ptr<Texture2> > Map;
			Map map;
			core::IdPool ids;
		};
	}
}

#endif