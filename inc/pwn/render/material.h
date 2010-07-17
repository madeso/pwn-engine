#ifndef PWN_RENDER_MATERIAL
#define PWN_RENDER_MATERIAL

#include <pwn/math/rgba.h>
#include <pwn/render/texture2.h>

#include <boost/shared_ptr.hpp>

namespace pwn
{
	namespace render
	{
		/** The material class that is used when rendering.
		 @todo remove Rgba colors and use a Rgb color, with a seperate member for alpha.
		 */
		class Material
		{
		public:
			Material();

			math::Rgba ambient; // -1 - 1
			math::Rgba diffuse; // -1 - 1
			math::Rgba specular; // -1 - 1
			math::Rgba emission; // -1 - 1
			real shininess; // 0-1

			bool hasTransperency;

			typedef boost::shared_ptr<render::Texture2> TexturePtr;
			TexturePtr texture;
			// color
			// maps: diffuse, normal/bump, emissive, specular
			// general shader?
		};
	}
}

#endif