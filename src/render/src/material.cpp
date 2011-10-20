#include <pwn/render/material.h>

namespace pwn
{
	namespace render
	{
		Material::Material()
			: ambient(0.2f)
			, diffuse(0.8f)
			, specular(0)
			, emission(0)
			, shininess(0)
		{
		}
	}
}
