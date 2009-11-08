#include <pwn/render/actordef>
#include <pwn/mesh/mesh>
#include <pwn/render/part>
#pragma warning(disable:4512) //'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>
#include <pwn/render/compiledmesh>

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
			ImmediateMode(boost::shared_ptr<SharedMesh> smesh, const mesh::Mesh& mesh)
				: smesh(smesh)
				, triangles(mesh.triangles)
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

		boost::shared_ptr<ActorDef> Compile(const mesh::Mesh& mesh)
		{
			boost::shared_ptr<ActorDef> def( new ActorDef() );
			boost::shared_ptr<SharedMesh> smesh( new SharedMesh(mesh) );

			ActorDef::PartPtr part( new Part() );
			// todo: implement better rendering
			part->mesh.reset( new ImmediateMode(smesh, mesh) );

			def->parts.push_back(part);
			return def;
		}
	}
}