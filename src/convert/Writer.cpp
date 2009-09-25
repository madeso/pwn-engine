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

			const pwn::math::vec2 cvec2(const math::vec3& v)
			{
				return pwn::math::vec2(v.x, v.y);
			}

			void Write(std::ofstream* f, const pwn::math::real& r, bool optimize)
			{
				if( optimize )
				{
					pwn::math::uint16 i = pwn::math::FloatToHalf(r);
					f->write(reinterpret_cast<const Byte*>(&i), sizeof(pwn::math::uint16));
				}
				else f->write(reinterpret_cast<const Byte*>(&r), sizeof(pwn::math::real));
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

		const pwn::math::real f(pwn::math::real r)
		{
			return pwn::math::HalfToFloat(pwn::math::FloatToHalf(r));
		}

		const pwn::math::real PosDiff(const pwn::math::vec3& v)
		{
			const pwn::math::vec3 loaded(f(v.x), f(v.y), f(v.z));
			//const pwn::math::vec3 loaded = pwn::math::CompressedToVector(pwn::math::VectorToCompressed(v));
			//const pwn::math::vec3 loaded = pwn::math::CompressedToUnitVector(pwn::math::UnitVectorToCompressed(pwn::math::GetNormalized(v))) * f(pwn::math::LengthOf(v));
			return (pwn::math::LengthOf(v-loaded) / pwn::math::LengthOf(v))*100;
		}

		const pwn::math::real AngleDiff(const pwn::math::vec3& v)
		{
			//const pwn::math::vec3 loaded(f(v.x), f(v.y), f(v.z));
			const pwn::math::vec3 loaded = pwn::math::CompressedToUnitVector(pwn::math::UnitVectorToCompressed(v));
			const pwn::math::real a = pwn::math::AngleBetween(v, loaded).inDegrees();
			return a;
		}

		void Test(Converter& data)
		{
			/* vertices */ {
				const std::size_t vc = data.vertices.size();
				pwn::math::real min = 100;
				pwn::math::real max = -100;
				pwn::math::real average = 0;
				for(std::size_t i=0; i<vc; ++i)
				{
					const pwn::math::real d = PosDiff(data.vertices[i]*200);
					average += d;
					if( min > d ) min = d;
					if( max < d ) max = d;
				}
				average = average / vc;
				std::cout << "Position (units):" << min << ", " << average << ", " << max << std::endl;
			}

			/* normals */ {
				const std::size_t nc = data.normals.size();
				pwn::math::real min = 100;
				pwn::math::real max = -100;
				pwn::math::real average = 0;
				for(std::size_t i=0; i<nc; ++i)
				{
					const pwn::math::real d = AngleDiff(data.normals[i]);
					average += d;
					if( min > d ) min = d;
					if( max < d ) max = d;
				}
				average = average / nc;
				std::cout << "Normal (degrees):" << min << ", " << average << ", " << max << std::endl;
			}
		}

		void Write(Converter& data, const pwn::core::string& file, bool optimize)
		{
			std::ofstream f(file.c_str(), std::ios::out | std::ios::binary);
			if( !f.good() ) throw "failed to open file for writing";
			f.exceptions ( std::ofstream::eofbit | std::ofstream::failbit | std::ofstream::badbit );

			/* header */ {
				pwn::math::uint16 version = 0;
				f.write( reinterpret_cast<const Byte*>(&version), sizeof(pwn::math::uint16));
				pwn::math::uint16 flags = optimize? 1 : 0;
				f.write( reinterpret_cast<const Byte*>(&flags), sizeof(pwn::math::uint16));
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
					Write(&f, cvec2(data.normals[i]), optimize);
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
						f.write(reinterpret_cast<const Byte*>(&data.faces[i].indices[faceIndex].vertex), sizeof(std::size_t) * 3);
					}
					f.write(reinterpret_cast<const Byte*>(&data.faces[i].material), sizeof(std::size_t));
				}
			}
		}
	}
}