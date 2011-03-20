//#pragma warning(disable:4512) // boost unable to generate assignment operator

#include <pwn/mesh/builder.h>
#include <pwn/mesh/mesh.h>
#include <boost/foreach.hpp>
#include <pwn/math/operations.h>
#include <pwn/core/stringutils.h>
#include <fstream>

#include <pwn/assert.h>

namespace pwn
{
	namespace mesh
	{
		Builder* Move(Builder* mesh, const math::vec3& dir)
		{
			BOOST_FOREACH(BPoint& p, mesh->positions)
			{
				p.location += dir;
			}

			return mesh;
		}

		Builder* Scale(Builder* mesh, pwn::real scale)
		{
			BOOST_FOREACH(BPoint& p, mesh->positions)
			{
				p.location *= scale;
			}

			BOOST_FOREACH(Bone& b, mesh->bones)
			{
				b.pos *= scale;
			}

			return mesh;
		}

		Builder* InvertNormals(Builder* mesh)
		{
			BOOST_FOREACH(math::vec3& p, mesh->normals)
			{
				p = -p;
			}

			return mesh;
		}



		namespace // local
		{
			BTriangle::Vertex v(BTriangle::index p, BTriangle::index t)
			{
				BTriangle::Vertex vt;
				vt.location = p;
				vt.texture = t;
				return vt;
			}
		}

		Builder CreateBox(Material material, real w, real h, real d, bool faceOut)
		{
			using math::vec2;
			using math::vec3;

			Builder b;

			const BTriangle::index t0 = b.addTextCoord(vec2(0,1));
			const BTriangle::index t1 = b.addTextCoord(vec2(1,1));
			const BTriangle::index t2 = b.addTextCoord(vec2(0,0));
			const BTriangle::index t3 = b.addTextCoord(vec2(1,0));

			const math::vec4 noBone(-1, -1, -1, -1);

			// front side
			const BTriangle::index v0 = b.addPosition(vec3(0, 0, 0), noBone);
			const BTriangle::index v1 = b.addPosition(vec3(w, 0, 0), noBone);
			const BTriangle::index v2 = b.addPosition(vec3(0, h, 0), noBone);
			const BTriangle::index v3 = b.addPosition(vec3(w, h, 0), noBone);

			// back side
			const BTriangle::index v4 = b.addPosition(vec3(0, 0, d), noBone);
			const BTriangle::index v5 = b.addPosition(vec3(w, 0, d), noBone);
			const BTriangle::index v6 = b.addPosition(vec3(0, h, d), noBone);
			const BTriangle::index v7 = b.addPosition(vec3(w, h, d), noBone);

			b.addQuad(!faceOut, 0, v(v0,t2), v(v2,t0), v(v3,t1), v(v1,t3)); // front
			b.addQuad(!faceOut, 0, v(v1,t3), v(v3,t1), v(v7,t0), v(v5,t2)); // right
			b.addQuad(!faceOut, 0, v(v4,t3), v(v6,t1), v(v2,t0), v(v0,t2)); // left
			b.addQuad(!faceOut, 0, v(v5,t2), v(v7,t0), v(v6,t1), v(v4,t3)); // back
			b.addQuad(!faceOut, 0, v(v3,t1), v(v2,t3), v(v6,t2), v(v7,t0)); // up
			b.addQuad(!faceOut, 0, v(v4,t0), v(v0,t1), v(v1,t3), v(v5,t2)); // bottom

			b.materials.push_back(material);
			return b;
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

		void MoveTextures(Builder* mesh, const pwn::string& newFolder)
		{
			for(std::size_t i = 0; i < mesh->materials.size(); ++i)
			{
				MoveTextures(mesh->materials[i], newFolder);
			}
		}

		// ==================================================================================================================================

		BPoint::BPoint(const math::vec3& alocation, math::vec4 abone)
			: location(alocation)
			, bone(abone)
		{
		}

		// ==================================================================================================================================

		BTriangle::Vertex::Vertex()
			: location(0)
			, texture(0)
			, normal(0)
		{
		}

		BTriangle::Vertex::Vertex(index vertex, index anormal, index textureCoordinate)
			: location(vertex)
			, texture(textureCoordinate)
			, normal(anormal)
		{
		}

		BTriangle::Vertex BTriangle::Vertex::Create_VN_T(index vertexAndNormal, index text)
		{
			return Vertex(vertexAndNormal, vertexAndNormal, text);
		}

		BTriangle::BTriangle(Vertex a, Vertex b, Vertex c)
			: v0(a), v1(b), v2(c)
		{
		}

		BTriangle::BTriangle()
		{
		}

		namespace // local
		{
			BTriangle::Vertex BTriangle::* const offsets[3] = {&BTriangle::v0, &BTriangle::v1, &BTriangle::v2};
		}

		BTriangle::Vertex& BTriangle::operator[](pwn::uint32 index)
		{
			return this->*offsets[index];
		}

		const BTriangle::Vertex& BTriangle::operator[](pwn::uint32 index) const
		{
			return this->*offsets[index];
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

		BTriangle::index Builder::addTextCoord(const math::vec2& tc)
		{
			texcoords.push_back(tc);
			return static_cast<BTriangle::index>(texcoords.size()-1);
		}

		BTriangle::index Builder::addPosition(const BPoint& pos)
		{
			positions.push_back(pos);
			return static_cast<BTriangle::index>(positions.size()-1);
		}

		BTriangle::index Builder::addPosition(const math::vec3& pos, math::vec4 bone)
		{
			return addPosition(BPoint(pos, bone));
		}

		BTriangle::index Builder::addNormal(const math::vec3& norm)
		{
			normals.push_back(norm);
			return static_cast<BTriangle::index>(normals.size()-1);
		}

		void Builder::addTriangle(pwn::uint32 material, const BTriangle& t)
		{
			triangles[material].push_back(t);
		}

		BTriangle::index Builder::addMaterial(Material m)
		{
			materials.push_back(m);
			return static_cast<BTriangle::index>(materials.size()-1);
		}

		BTriangle::index Builder::getMaterial(const pwn::string& name) const
		{
			MaterialNameIdMap::const_iterator res = materialid.find(name);
			if( res == materialid.end() ) throw "unable to find material..";
			else return res->second;
		}

		BTriangle::index Builder::addMaterial(const pwn::string& name, Material m)
		{
			const BTriangle::index index = addMaterial(m);
			materialid[name] = index;
			return index;
		}

		void Builder::addQuad(bool reverse, pwn::uint32 material, const BTriangle::Vertex& v0, const BTriangle::Vertex& v1, const BTriangle::Vertex& v2, const BTriangle::Vertex& v3)
		{
			if( reverse )
			{
				addTriangle(material, BTriangle(v2, v1, v0) );
				addTriangle(material, BTriangle(v3, v2, v0) );
			}
			else
			{
				addTriangle(material, BTriangle(v0, v1, v2) );
				addTriangle(material, BTriangle(v0, v2, v3) );
			}
		}

		void Builder::addFace(pwn::uint32 material, const std::vector<BTriangle::Vertex>& vertices)
		{
			// we currently doesnt support ton-triangular faces so - triangulate it
			const std::vector<BTriangle::Vertex>::size_type size = vertices.size();
			bool added = false;
			for(std::vector<BTriangle::Vertex>::size_type i=2; i<size; ++i)
			{
				addTriangle(material, BTriangle(vertices[0], vertices[i-1], vertices[i]));
				added = true;
			}
			if( false == added ) throw "Unable to triangulate face";
		}

		void Builder::addBone(const Bone& b)
		{
			bones.push_back(b);
		}

		void Builder::buildNormals()
		{
			//Assert(normals.empty());
			using math::vec3;
			std::vector<vec3> vertexNormalsSum(positions.size(), vec3(0,0,0));
			BOOST_FOREACH(TriMap::value_type& tr, triangles)
			{
				BOOST_FOREACH(BTriangle& t, tr.second)
				{
					const vec3 p0 = positions[t[0].location].location;
					const vec3 p1 = positions[t[1].location].location;
					const vec3 p2 = positions[t[2].location].location;

					const vec3 d0 = math::FromTo(p1, p0);
					const vec3 d1 = math::FromTo(p1, p2);

					const vec3 faceNormal = math::crossNorm(d1, d0);

					for(int i=0; i<3; ++i)
					{
						vertexNormalsSum[t[i].location] += faceNormal;
						t[i].normal = t[i].location;
					}
				}
			}

			normals.clear();
			BOOST_FOREACH(const vec3& normalSum, vertexNormalsSum)
			{
				normals.push_back( math::GetNormalized(normalSum) );
			}

			Assert(normals.size() == positions.size());
		}

		namespace // local
		{
			using namespace pwn::math;
			struct Data
			{
				mat44 globalskel;
			};

			void PrepareVericesForAnimation(Mesh* mesh)
			{
				using namespace pwn;
				using namespace pwn::mesh;
				using namespace pwn::math;

				std::vector<Data> bdp(mesh->getBones().size());

				for (unsigned int i = 0; i < mesh->getBones().size(); ++i)
				{
					const Bone& bone = mesh->getBones()[i];
					Assert(!bone.hasParent() || bone.getParent() < i); // if it has a parent, it should already have been processed
					const mat44 local = mat44helper(mat44Identity()).translate(bone.pos).rotate(GetConjugate(bone.rot)).mat;
					const mat44 parent = bone.hasParent() ? bdp[bone.getParent()].globalskel : mat44Identity();
					const mat44 global = parent*local;
					bdp[i].globalskel = global;
				}

				for(unsigned int i=0; i < mesh->data().getCount(); ++i)
				{
					const Point& p = mesh->data().getPoint(i);
					if( p.hasBone() == false) continue;
					const math::vec4 bone = p.getBone();

					vec3 location(0,0,0);
					vec3 normal(0,0,0);

					real insum = 0;

					for(int b=0; b<4; ++b)
					{
						const real x = bone[b];
						if( x < 0 ) break;
						const real in = GetBoneInfluence(x);
						insum += in;
						Data& data = bdp[GetBoneIndex(x)];
						location += in * TranslateWithInverseMatrix(p.location, data.globalskel);
						normal += in * GetNormalized(TranslateWithInverseMatrix(p.normal, math::SetTransform(data.globalskel, math::vec3(0,0,0))));
					}
					const real inv = 1/insum;
					mesh->setLocationNormal(i, inv*location, GetNormalized(math::vec3(inv*normal)));
				}
			}
		}

		struct Combo
		{
			Triangle::VertexIndex location;
			Triangle::VertexIndex texture;
			Triangle::VertexIndex normal;
			math::vec4 boneIndex;

			Combo(const BTriangle::Vertex& v, math::vec4 bi)
				: location(v.location)
				, texture(v.texture)
				, normal(v.normal)
				, boneIndex(bi)
			{
			}
		};

		bool operator<(const Combo& lhs, const Combo& rhs)
		{
#define TEST(x) if( lhs.x != rhs.x ) return lhs.x < rhs.x
			TEST(location);
			else TEST(texture);
			else TEST(normal);
			else TEST(boneIndex);
			else
			{
				// all equal
				return false;
			}
#undef TEST
		}

		Mesh Builder::asMesh() const
		{
			typedef std::map<Combo, Triangle::VertexIndex> ComboMap;
			ComboMap combinations;

			// foreach triangle
			std::vector<math::vec3> posv;
			std::vector<math::vec3> normv;
			std::vector<math::vec2> textv;
			std::vector<math::vec4> bonev;
			Mesh::TriangleMap trim;
			BOOST_FOREACH(const TriMap::value_type& triangleMaterial, triangles)
			{
				const pwn::uint32 material = triangleMaterial.first;
				BOOST_FOREACH(const BTriangle& sourceTriangle, triangleMaterial.second)
				{
					Triangle::VertexIndex triangle[3];

					// get combinations, add if not existing
					for(int i=0; i<3; ++i)
					{
						const math::vec4 boneIndex = positions[sourceTriangle[i].location].bone;
						const Combo c(sourceTriangle[i], boneIndex);
						ComboMap::iterator result = combinations.find(c);
						if( result != combinations.end() )
						{
							triangle[i] = result->second;
						}
						else
						{
							const math::vec3 pos = positions[c.location].location;
							const math::vec2 text = texcoords[c.texture];
							const math::vec3 normal = normals.empty() == false ? normals[c.normal] : math::vec3(0,0,0);
							Triangle::VertexIndex ind = posv.size();
							posv.push_back(pos);
							textv.push_back(text);
							normv.push_back(normal);
							bonev.push_back(boneIndex);
							combinations.insert( ComboMap::value_type(c, ind) );
							triangle[i] = ind;
						}
					}

					// add traingle to mesh
					trim[material].push_back(triangle);
				}
			}

			Mesh mesh(posv, normv, textv, bonev, trim, bones, materials);
			Assert(mesh.validate(false) == 0);

			PrepareVericesForAnimation(&mesh);

			if(mesh.validate(true) != 0 ) throw "Mesh failed to validate";
			return mesh;
		}

		class BoneToSort
		{
		public:
			BoneIndex index;
			BoneToSort* parent;
			std::vector<BoneToSort*> children;

			void traverse(std::vector<BoneIndex>* list) const
			{
				list->push_back(index);
				BOOST_FOREACH(BoneToSort* b, children)
				{
					b->traverse(list);
				}
			}
		};

		Flatouter::Flatouter(const Builder& mesh)
		{
			/*
			std::vector<BoneToSort> bones;
			for(BoneIndex i=0; i<mesh.bones.size(); ++i)
			{
				BoneToSort b;
				b.index = i;
				b.parent = 0;
				bones.push_back(b);
			}
			for(BoneIndex i=0; i<bones.size(); ++i)
			{
				if( mesh.bones[i].hasParent() )
				{
					bones[i].parent = &bones[mesh.bones[i].getParent()];
				}
			}
			for(BoneIndex i=0; i<bones.size(); ++i)
			{
				if( bones[i].parent )
				{
					bones[i].parent->children.push_back(&bones[i]);
				}
			}
			for(BoneIndex i=0; i<mesh.bones.size(); ++i)
			{
				if( bones[i].parent == 0 )
				{
					bones[i].traverse(&newIndices);
				}
			}*/
		}

		void Flatouter::modify(Builder* mesh) const
		{
			if( newIndices.empty() ) return;
			BOOST_FOREACH(BPoint& p, mesh->positions)
			{
				for(int i=0; i<4; ++i)
				{
					SetBoneIndex(&p.bone[i], newIndices[ GetBoneIndex(p.bone[i]) ]);
				}
			}

			std::vector<Bone> bs = mesh->bones;
			for(pwn::uint32 i=0; i<mesh->bones.size(); ++i)
			{
				mesh->bones[i] = bs[newIndices[i]];
				Bone& b = mesh->bones[i];
				if( b.hasParent() )
				{
					b.setParent(newIndices[b.getParent()]);
				}
			}
		}

		void Flatouter::modify(Animation* animation) const
		{
			if( newIndices.empty() ) return;

			std::vector<AnimationPerBone> apb = animation->bones;
			for(pwn::uint32 i=0; i<animation->bones.size(); ++i)
			{
				animation->bones[i] = apb[newIndices[i]];
			}
		}
	}
}
