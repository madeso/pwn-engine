#ifndef PWN_CONVERT_CONVERTER_HPP
#define PWN_CONVERT_CONVERTER_HPP

#include <vector>
#include <map>

#include <pwn/math/types>
#include <pwn/core/string>

namespace pwn
{
	namespace convert
	{
		struct FaceIndex
		{
			std::size_t vertex;
			std::size_t normal;
			std::size_t textureCoordiante;
		};

		struct Face
		{
			FaceIndex indices[3];
			std::size_t material;
		};

		struct Material
		{
			Material();
			pwn::core::string name; // may be empty
			pwn::math::Rgba ambient;
			pwn::math::Rgba diffuse;
			pwn::math::Rgba specular;
			pwn::math::Rgba emissive;
			pwn::math::real shininess;

			pwn::core::string textureDiffuse;
		};

		// sucky name, I realy should rename it to model or something
		class Converter
		{
		public:
			void addVertex(const pwn::math::vec3& p);
			void addTextureCoordinate(const pwn::math::vec2& tex);
			void addNormal(const pwn::math::vec3& n);
			void addFace(const std::size_t material, const std::vector<FaceIndex>& points);
			void addFace(const Face& face);
			const std::size_t addMaterial(const Material& material);

		public:
			std::vector<pwn::math::vec3> vertices;
			std::vector<pwn::math::vec2> textureCoordinates;
			std::vector<pwn::math::vec3> normals;
			std::vector<Material> materials;
			std::vector<Face> faces;

			typedef pwn::math::uint16 NormalIndex;
			typedef pwn::math::uint16 CompressedNormal;
			std::map<CompressedNormal, NormalIndex> normalMap;
			std::vector<NormalIndex> normalConvertions;
		};
	}
}

#endif