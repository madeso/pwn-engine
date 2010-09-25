#pragma warning(disable:4512) // boost unable to generate assignment operator

#include <pwn/mesh/builder.h>
#include <pwn/mesh/mesh.h>
#include <boost/foreach.hpp>
#include <pwn/math/operations.h>
#include <pwn/core/StringUtils.h>
#include <fstream>

#include <pwn/core/stdutil.h>
namespace pwn
{
	namespace mesh
	{
		Mesh* Move(Mesh* mesh, const math::vec3& dir)
		{
			BOOST_FOREACH(Point& p, mesh->positions)
			{
				p.location += dir;
			}

			return mesh;
		}

		Mesh* Scale(Mesh* mesh, pwn::real scale)
		{
			BOOST_FOREACH(Point& p, mesh->positions)
			{
				p.location *= scale;
			}

			return mesh;
		}

		Mesh* InvertNormals(Mesh* mesh)
		{
			BOOST_FOREACH(math::vec3& n, mesh->normals)
			{
				n = -n;
			}

			return mesh;
		}

		namespace
		{
			void KeepLast(pwn::string& t, const char c)
			{
				std::size_t i = t.find_last_of(c);
				if( i != pwn::string::npos )
				{
					t = t.substr(i+1);
				}
			}

			pwn::string MoveTexture(const pwn::string& texture, const pwn::string& newFolder)
			{
				if( pwn::core::Trim(texture) == "" ) return "";
				pwn::string t = texture;
				KeepLast(t, '\\');
				KeepLast(t, '/');
				KeepLast(t, '|');
				if( pwn::core::EndsWith(t, ".tif") ) t = pwn::core::StringReplace(t, ".tif", ".png");
				if( newFolder.empty() ) return t;
				else return newFolder + "/" + t;
			}

			void MoveTextures(Material& mat, const pwn::string& newFolder)
			{
				mat.setTexture_Diffuse( MoveTexture(mat.texture_diffuse, newFolder) );
			}
		}

		void MoveTextures(Mesh* mesh, const pwn::string& newFolder)
		{
			for(std::size_t i = 0; i < mesh->materials.size(); ++i)
			{
				MoveTextures(mesh->materials[i], newFolder);
			}
		}

		// ==================================================================================================================================

		namespace // local
		{
			Triangle::Vertex v(Triangle::index p, Triangle::index t)
			{
				return Triangle::Vertex(p, 0, t);
			}
		}

		// ==================================================================================================================================

		void Builder::clear()
		{
			triangles.clear();
			positions.clear();
			normals.clear();
			texcoords.clear();
			bones.clear();
			materials.clear();
		}

		Triangle::index Builder::addTextCoord(const math::vec2& tc)
		{
			texcoords.push_back(tc);
			return static_cast<Triangle::index>(texcoords.size()-1);
		}

		Triangle::index Builder::addPosition(const Point& pos)
		{
			positions.push_back(pos);
			return static_cast<Triangle::index>(positions.size()-1);
		}

		Triangle::index Builder::addPosition(const math::vec3& pos, BoneIndex bone)
		{
			return addPosition(Point(pos, bone));
		}
		
		Triangle::index Builder::addNormal(const math::vec3& norm)
		{
			normals.push_back(norm);
			return static_cast<Triangle::index>(normals.size()-1);
		}

		void Builder::addTriangle(pwn::uint32 material, const Triangle& t)
		{
			triangles[material].push_back(t);
		}

		Triangle::index Builder::addMaterial(Material m)
		{
			materials.push_back(m);
			return static_cast<Triangle::index>(materials.size()-1);
		}
		
		void Builder::addQuad(bool reverse, pwn::uint32 material, const Triangle::Vertex& v0, const Triangle::Vertex& v1, const Triangle::Vertex& v2, const Triangle::Vertex& v3)
		{
			if( reverse )
			{
				addTriangle(material, Triangle(v2, v1, v0) );
				addTriangle(material, Triangle(v3, v2, v0) );
			}
			else
			{
				addTriangle(material, Triangle(v0, v1, v2) );
				addTriangle(material, Triangle(v0, v2, v3) );
			}
		}

		void Builder::addFace(pwn::uint32 material, const std::vector<Triangle::Vertex>& vertices)
		{
			// we currently doesnt support ton-triangular faces so - triangulate it
			const std::vector<Triangle::Vertex>::size_type size = vertices.size();
			bool added = false;
			for(std::vector<Triangle::Vertex>::size_type i=2; i<size; ++i)
			{
				addTriangle(material, Triangle(vertices[0], vertices[i-1], vertices[i]));
				added = true;
			}
			if( false == added ) throw "Unable to triangulate face";
		}
		
		void Builder::setBox(Material material, real w, real h, real d, bool faceOut)
		{
			using math::vec2;
			using math::vec3;

			clear();

			const Triangle::index t0 = addTextCoord(vec2(0,1));
			const Triangle::index t1 = addTextCoord(vec2(1,1));
			const Triangle::index t2 = addTextCoord(vec2(0,0));
			const Triangle::index t3 = addTextCoord(vec2(1,0));

			// front side
			const Triangle::index v0 = addPosition(vec3(0, 0, 0), 0);
			const Triangle::index v1 = addPosition(vec3(w, 0, 0), 0);
			const Triangle::index v2 = addPosition(vec3(0, h, 0), 0);
			const Triangle::index v3 = addPosition(vec3(w, h, 0), 0);

			// back side
			const Triangle::index v4 = addPosition(vec3(0, 0, d), 0);
			const Triangle::index v5 = addPosition(vec3(w, 0, d), 0);
			const Triangle::index v6 = addPosition(vec3(0, h, d), 0);
			const Triangle::index v7 = addPosition(vec3(w, h, d), 0);

			addQuad(!faceOut, 0, v(v0,t2), v(v2,t0), v(v3,t1), v(v1,t3)); // front
			addQuad(!faceOut, 0, v(v1,t3), v(v3,t1), v(v7,t0), v(v5,t2)); // right
			addQuad(!faceOut, 0, v(v4,t3), v(v6,t1), v(v2,t0), v(v0,t2)); // left
			addQuad(!faceOut, 0, v(v5,t2), v(v7,t0), v(v6,t1), v(v4,t3)); // back
			addQuad(!faceOut, 0, v(v3,t1), v(v2,t3), v(v6,t2), v(v7,t0)); // up
			addQuad(!faceOut, 0, v(v4,t0), v(v0,t1), v(v1,t3), v(v5,t2)); // bottom

			materials.push_back(material);
		}

		void Builder::buildNormals()
		{
			using math::vec3;
			std::vector<vec3> vertexNormalsSum(positions.size(), vec3(0,0,0));
			BOOST_FOREACH(TriMap::value_type& tr, triangles)
			{
				BOOST_FOREACH(Triangle& t, tr.second)
				{
					const vec3 p0 = positions[t[0].location].location;
					const vec3 p1 = positions[t[1].location].location;
					const vec3 p2 = positions[t[2].location].location;

					const vec3 d0 = math::FromTo(p1, p0);
					const vec3 d1 = math::FromTo(p1, p2);

					const vec3 faceNormal = math::crossNorm(d0, d1);

					vertexNormalsSum[t[0].location] += faceNormal;
					vertexNormalsSum[t[1].location] += faceNormal;
					vertexNormalsSum[t[2].location] += faceNormal;

					for(int i=0; i<3; ++i) t[i].normal = t[i].location;
				}
			}

			normals.clear();
			BOOST_FOREACH(const vec3& normalSum, vertexNormalsSum)
			{
				normals.push_back( math::GetNormalized(normalSum) );
			}
		}

		namespace // local
		{
			using namespace pwn::math;
			struct Data
			{
				mat44 globalskel;
			};

			vec3 VectorIRotate(const vec3 vec, const mat44& mat)
			{
				return vec3(
					vec.x*mat.at(0,0) + vec.y*mat.at(1,0) + vec.z*mat.at(2,0),
					vec.x*mat.at(0,1) + vec.y*mat.at(1,1) + vec.z*mat.at(2,1),
					vec.x*mat.at(0,2) + vec.y*mat.at(1,2) + vec.z*mat.at(2,2));
			}

			vec3 VectorITransform (const vec3 vec, const mat44& mat)
			{
				vec3 tmp(
					vec.x - mat.at(0,3),
					vec.y - mat.at(1,3),
					vec.z - mat.at(2,3));
				return VectorIRotate(tmp, mat);
			}

			void PrepareVericesForAnimation(Mesh* mesh)
			{
				using namespace pwn;
				using namespace pwn::mesh;
				using namespace pwn::math;

				std::vector<Data> bdp(mesh->bones.size());

				for (unsigned int i = 0; i < mesh->bones.size(); ++i)
				{
					Bone& bone = mesh->bones[i];
					
					const mat44 local = mat44helper(mat44Identity()).rotate(GetConjugate(bone.rot)).translate(bone.pos).mat;
					const mat44 parent = bone.hasParent() ? bdp[bone.getParent()].globalskel : mat44Identity();
					const mat44 global = parent*local;
					bdp[i].globalskel = global;
				}

				for(unsigned int i=0; i < mesh->positions.size(); ++i)
				{
					Point& p = mesh->positions[i];
					if( p.hasBone() == false) continue;
					Data& data = bdp[p.getBone()];
					vec3 ms3d = VectorITransform(p.location, data.globalskel);
					p.location = ms3d;
				}
			}
		}

		bool Builder::makeMesh(Mesh& mesh, Flatouter* flatouter) const
		{
			mesh.clear();

			mesh.positions = positions;
			mesh.normals = normals;
			mesh.texcoords = texcoords;
			mesh.bones = bones;
			mesh.triangles = triangles;
			mesh.materials = materials;

			if( flatouter )
			{
				flatouter->load(mesh);
				flatouter->modify(&mesh);
			}

			PrepareVericesForAnimation(&mesh);

			return mesh.validate() ==0;
		}

		uint32 NumberOfTriangles(const Mesh& mesh)
		{
			uint32 count = 0;
			for(Mesh::TriangleMap::const_iterator i = mesh.triangles.begin(); i != mesh.triangles.end(); ++i)
			{
				count += i->second.size();
			}
			return count;
		}
	}
}
