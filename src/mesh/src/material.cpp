#include <pwn/mesh/predefinedmaterials.h>
#include <pwn/core/stringutils.h>

namespace pwn
{
	namespace mesh
	{
		Material::Material()
			: ambient(0.2f)
			, diffuse(0.8f)
			, specular(0)
			, emission(0)
			, shininess(0)
		{
			texture_diffuse[0] = 0;
		}

		const string Material::getTexture_Diffuse() const
		{
			return texture_diffuse;
		}

		void Material::setTexture_Diffuse(const pwn::string& v)
		{
			core::Copy(texture_diffuse, v, TextureSize);
		}

		namespace materials
		{
			namespace
			{
				Material CreateColor(double ar, double ag, double ab, double dr, double dg, double db, double sr, double sg, double sb, double s)
				{
					Material m;

					// I could fix theese warning by adding casts, but that would be too unreadable...
#pragma warning(push)
#pragma warning(disable:4244) // 'argument' : conversion from 'double' to 'const pwn::real', possible loss of data
					m.ambient.rgb(ar, ag, ab);
					m.diffuse.rgb(dr, dg, db);
					m.specular.rgb(sr, sg, sb);
#pragma warning(pop)
					m.shininess = static_cast<real>(s);

					return m;
				}
			}

			// These numbers come from the OpenGL teapots.c demo, © Silicon Graphics, Inc., © 1994, Mark J. Kilgard
			// http://devernay.free.fr/cours/opengl/materials.html
			// todo: come up with own

			Material Emerald()
			{
				return CreateColor(0.0215, 0.1745, 0.0215, 0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 0.6);
			}
			Material Jade()
			{
				return CreateColor(0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 0.1);
			}
			Material Obsidian()
			{
				return CreateColor(0.05375, 0.05, 0.06625, 0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 0.3);
			}
			Material Pearl()
			{
				return CreateColor(0.25, 0.20725, 0.20725, 1, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 0.088);
			}
			Material Ruby()
			{
				return CreateColor(0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 0.6);
			}
			Material Turquoise()
			{
				return CreateColor(0.1, 0.18725, 0.1745, 0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 0.1);
			}
			Material Brass()
			{
				return CreateColor(0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 0.941176, 0.807843, 0.21794872);
			}
			Material Bronze()
			{
				return CreateColor(0.2125, 0.1275, 0.054, 0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 0.2);
			}
			Material Chrome()
			{
				return CreateColor(0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 0.6);
			}
			Material Copper()
			{
				return CreateColor(0.19125, 0.0735, 0.0225, 0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 0.1);
			}
			Material Gold()
			{
				return CreateColor(0.24725, 0.1995, 0.0745, 0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 0.4);
			}
			Material Silver()
			{
				return CreateColor(0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4);
			}
			Material Plastic_Black()
			{
				return CreateColor(0.0, 0.0, 0.0, 0.01, 0.01, 0.01, 0.50, 0.50, 0.50, .25);
			}
			Material Plastic_Cyan()
			{
				return CreateColor(0.0, 0.1, 0.06, 0.0, 0.50980392, 0.50980392, 0.50196078, 0.50196078, 0.50196078, .25);
			}
			Material Plastic_Green()
			{
				return CreateColor(0.0, 0.0, 0.0, 0.1, 0.35, 0.1, 0.45, 0.55, 0.45, .25);
			}
			Material Plastic_Red()
			{
				return CreateColor(0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.7, 0.6, 0.6, .25);
			}
			Material Plastic_White()
			{
				return CreateColor(0.0, 0.0, 0.0, 0.55, 0.55, 0.55, 0.70, 0.70, 0.70, .25);
			}
			Material Plastic_Yellow()
			{
				return CreateColor(0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.60, 0.60, 0.50, .25);
			}
			Material Rubber_Black()
			{
				return CreateColor(0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.4, 0.4, 0.4, .078125);
			}
			Material Rubber_Cyan()
			{
				return CreateColor(0.0, 0.05, 0.05, 0.4, 0.5, 0.5, 0.04, 0.7, 0.7, .078125);
			}
			Material Rubber_Green()
			{
				return CreateColor(0.0, 0.05, 0.0, 0.4, 0.5, 0.4, 0.04, 0.7, 0.04, .078125);
			}
			Material Rubber_Red()
			{
				return CreateColor(0.05, 0.0, 0.0, 0.5, 0.4, 0.4, 0.7, 0.04, 0.04, .078125);
			}
			Material Rubber_White()
			{
				return CreateColor(0.05, 0.05, 0.05, 0.5, 0.5, 0.5, 0.7, 0.7, 0.7, .078125);
			}
			Material Rubber_Yellow()
			{
				return CreateColor(0.05, 0.05, 0.0, 0.5, 0.5, 0.4, 0.7, 0.7, 0.04, .078125);
			}
			Material White()
			{
				return CreateColor(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0);
			}
		}
	}
}
