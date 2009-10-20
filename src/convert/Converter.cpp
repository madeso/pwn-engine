#include "Converter.hpp"

#include <pwn/math/operations>

namespace pwn
{
	namespace convert
	{
		Material::Material()
			: name("")
			, ambient(0.2f, 0.2f, 0.2f, 1.0f)
			, diffuse(0.8f, 0.8f, 0.8f, 1.0f)
			, specular(0, 0, 0, 1)
			, emissive(0, 0, 0, 1)
			, shininess(0)
			, textureDiffuse("")
		{
		}

		void Converter::addVertex(const pwn::math::vec3& p)
		{
			vertices.push_back(p);
		}

		void Converter::addTextureCoordinate(const pwn::math::vec2& tex)
		{
			textureCoordinates.push_back(tex);
		}

		void Converter::addNormal(const pwn::math::vec3& n)
		{
			const pwn::math::uint16 c = pwn::math::UnitVectorToCompressed(n);

			if( normalMap.find(c) == normalMap.end() )
			{
				normals.push_back(n);
				normalMap[c] = normalConvertions.size() +1;
			}

			normalConvertions.push_back(normalMap[c]);
		}

		const std::size_t Converter::addMaterial(const Material& material)
		{
			const std::size_t index = materials.size();
			materials.push_back(material);
			return index;
		}

		void Converter::addFace(const std::size_t material, const std::vector<FaceIndex>& points)
		{
			// we currently doesnt support ton-triangular faces so - triangulate it
			const std::vector<FaceIndex>::size_type size = points.size();
			bool added = false;
			for(std::vector<FaceIndex>::size_type i=2; i<size; ++i)
			{
				Face f;
				f.indices[0] = points[0];
				f.indices[1] = points[1];
				f.indices[2] = points[i];
				f.material = material;
				addFace(f);
				added = true;
			}
			if( false == added ) throw "Unable to triangulate face";
		}

		void Converter::addFace(const Face& face)
		{
			faces.push_back(face);
		}
	}
}