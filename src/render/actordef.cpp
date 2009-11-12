#include <pwn/render/actordef>
#include <pwn/mesh/mesh>
#include <pwn/render/part>
#pragma warning(disable:4512) //'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>
#include <pwn/render/compiledmesh>
#include <pwn/render/material>
#include <pwn/mesh/material>

#include <sfml/OpenGl.hpp>

namespace pwn
{
	namespace render
	{
		ActorDef::ActorDef()
		{
		}

		ActorDef::~ActorDef()
		{
		}

		class SharedMesh
		{
		public:
			explicit SharedMesh(const mesh::Mesh& mesh)
				: positions(mesh.positions)
				, normals(mesh.normals)
				, texcoords(mesh.texcoords)
			{
			}

			std::vector<math::vec3> positions;
			std::vector<math::vec3> normals;
			std::vector<math::vec2> texcoords;
		};

		class ImmediateMode : public CompiledMesh
		{
		public:
			ImmediateMode(boost::shared_ptr<SharedMesh> smesh, const mesh::Mesh::TriangleList& tridata)
				: smesh(smesh)
				, triangles(tridata)
			{
			}

			~ImmediateMode()
			{
			}

			void render()
			{
				glBegin(GL_TRIANGLES);

				BOOST_FOREACH(const mesh::Triangle& tri, triangles)
				{
					const std::size_t nc = smesh->normals.size();
					for(int i=0; i<3; ++i)
					{
						if( nc != 0 )
						{
							const math::vec3 n = smesh->normals[tri[i].normal];
							glNormal3f(n.x, n.y, n.z);
						}
						const math::vec3 p = smesh->positions[tri[i].location];
						glVertex3f(p.x, p.y, p.z);
					}
				}

				glEnd();
			}
		private:
			boost::shared_ptr<SharedMesh> smesh;
			std::vector<mesh::Triangle> triangles;
		};

		boost::shared_ptr<render::Material> Compile(boost::shared_ptr<mesh::Material> mm)
		{
			boost::shared_ptr<render::Material> rm( new render::Material() );
			rm->ambient = mm->ambient;
			rm->diffuse = mm->diffuse;
			rm->specular = mm->specular;
			rm->emission = mm->emission;
			rm->shininess = mm->shininess;
			return rm;
		}

		boost::shared_ptr<ActorDef> Compile(const mesh::Mesh& mesh)
		{
			boost::shared_ptr<ActorDef> def( new ActorDef() );
			boost::shared_ptr<SharedMesh> smesh( new SharedMesh(mesh) );

			typedef std::map<pwn::uint32, mesh::Mesh::TriangleList> TriangleMap;
			TriangleMap triangles;

			BOOST_FOREACH(const mesh::Triangle& t, mesh.triangles)
			{
				triangles[t.material].push_back(t);
			}

			// todo: implement better rendering
			BOOST_FOREACH(TriangleMap::const_reference r, triangles)
			{
				ActorDef::PartPtr part( new Part() );
				const pwn::uint32 index = r.first;
				part->material = Compile( mesh.materials[index] );
				part->mesh.reset( new ImmediateMode(smesh, r.second) );
				def->parts.push_back(part);
			}
			return def;
		}
	}
}