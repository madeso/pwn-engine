#include "Writer.hpp"

#include <fstream>
#include "Converter.hpp"
#include <pwn/math/operations>

#include <iostream>

namespace pwn
{
	namespace convert
	{
		namespace // local
		{
			typedef char Byte;

			void Write(std::ofstream* f, const pwn::uint16 i)
			{
				f->write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint16));
			}
			void Write(std::ofstream* f, const pwn::uint32 i)
			{
				f->write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::uint32));
			}
			void Write(std::ofstream* f, const pwn::real& r, bool optimize)
			{
				if( optimize )
				{
					Write(f, pwn::math::FloatToHalf(r));
				}
				else f->write(reinterpret_cast<const Byte*>(&r), sizeof(pwn::real));
			}
			void Write(std::ofstream* f, const pwn::math::Rgba& c, bool optimize)
			{
				Write(f, c.r, optimize);
				Write(f, c.g, optimize);
				Write(f, c.b, optimize);
				Write(f, c.a, optimize);
			}
			void Write(std::ofstream* f, const pwn::math::vec3& v, bool optimize)
			{
				Write(f, v.x, optimize);
				Write(f, v.y, optimize);
				Write(f, v.z, optimize);
			}
			void Write(std::ofstream* f, const pwn::math::vec2& v, bool optimize)
			{
				Write(f, v.x, optimize);
				Write(f, v.y, optimize);
			}
		}

		const pwn::real CompressAndUncompress(pwn::real r)
		{
			return pwn::math::HalfToFloat(pwn::math::FloatToHalf(r));
		}

		const pwn::real PosDiff(const pwn::math::vec3& v)
		{
			const pwn::math::vec3 loaded(CompressAndUncompress(v.x), CompressAndUncompress(v.y), CompressAndUncompress(v.z));
			return (pwn::math::LengthOf(v-loaded) / pwn::math::LengthOf(v))*100;
		}

		const pwn::real AngleDiff(const pwn::math::vec3& v)
		{
			const pwn::math::vec3 loaded = pwn::math::CompressedToUnitVector(pwn::math::UnitVectorToCompressed(v));
			const pwn::real a = pwn::math::AngleBetween(v, loaded).inDegrees();
			return a;
		}

		void Test(Converter& data)
		{
			std::cout << "Normal: " << data.normalMap.size() << ", " << data.normalConvertions.size() << std::endl;
			/* vertices */ {
				const std::size_t vc = data.vertices.size();
				pwn::real min = 100;
				pwn::real max = -100;
				pwn::real average = 0;
				for(std::size_t i=0; i<vc; ++i)
				{
					const pwn::real d = PosDiff(data.vertices[i]*200);
					average += d;
					if( min > d ) min = d;
					if( max < d ) max = d;
				}
				average = average / vc;
				std::cout << "Position (percent): " << min << ", " << average << ", " << max << std::endl;
			}

			/* normals */ {
				const std::size_t nc = data.normals.size();
				pwn::real min = 100;
				pwn::real max = -100;
				pwn::real average = 0;
				for(std::size_t i=0; i<nc; ++i)
				{
					const pwn::real d = AngleDiff(data.normals[i]);
					average += d;
					if( min > d ) min = d;
					if( max < d ) max = d;
				}
				average = average / nc;
				std::cout << "Normal (degrees): " << min << ", " << average << ", " << max << std::endl;
			}
		}

		void Write(Converter& data, const pwn::string& file, bool optimize)
		{
			std::ofstream f(file.c_str(), std::ios::out | std::ios::binary);
			if( !f.good() ) throw "failed to open file for writing";
			f.exceptions ( std::ofstream::eofbit | std::ofstream::failbit | std::ofstream::badbit );

			/* header */ {
				pwn::uint16 version = 0;
				f.write( reinterpret_cast<const Byte*>(&version), sizeof(pwn::uint16));
				pwn::uint16 flags = optimize? 1 : 0;
				f.write( reinterpret_cast<const Byte*>(&flags), sizeof(pwn::uint16));
			}

			/* vertices */ {
				const std::size_t vc = data.vertices.size();
				f.write( reinterpret_cast<const Byte*>(&vc), sizeof(std::size_t));
				for(std::size_t i=0; i<vc; ++i)
				{
					Write(&f, data.vertices[i], optimize);
				}
			}

			/* texture coordinates */ {
				const std::size_t tc = data.textureCoordinates.size();
				f.write(reinterpret_cast<const Byte*>(&tc), sizeof(std::size_t));
				for(std::size_t i=0; i<tc; ++i)
				{
					Write(&f, data.textureCoordinates[i], optimize);
				}
			}

			/* normals */ {
				const std::size_t nc = data.normals.size();
				f.write(reinterpret_cast<const Byte*>(&nc), sizeof(std::size_t));
				for(std::size_t i=0; i<nc; ++i)
				{
					Write(&f, pwn::math::UnitVectorToCompressed(data.normals[i]));
				}
			}

			/* materials */ {
				const std::size_t mc = data.materials.size();
				f.write(reinterpret_cast<const Byte*>(&mc), sizeof(std::size_t));
				for(std::size_t i=0; i<mc; ++i)
				{
					Write(&f, data.materials[i].ambient, optimize);
					Write(&f, data.materials[i].diffuse, optimize);
					Write(&f, data.materials[i].specular, optimize);
					Write(&f, data.materials[i].emissive, optimize);
					Write(&f, data.materials[i].shininess, optimize);
				}
			}

			/* faces */ {
				const std::size_t fc = data.faces.size();
				f.write(reinterpret_cast<const Byte*>(&fc), sizeof(std::size_t));
				for(std::size_t i=0; i<fc; ++i)
				{
					for(int faceIndex=0; faceIndex<3; ++faceIndex)
					{
						// todo: remove hacky assumption of that the vertex is the first one
						Write(&f, data.faces[i].indices[faceIndex].vertex);
						Write(&f, data.normalConvertions[ static_cast<Converter::NormalIndex>(data.faces[i].indices[faceIndex].normal)]);
						Write(&f, data.faces[i].indices[faceIndex].textureCoordiante);
					}
					f.write(reinterpret_cast<const Byte*>(&data.faces[i].material), sizeof(std::size_t));
				}
			}
		}
	}
}