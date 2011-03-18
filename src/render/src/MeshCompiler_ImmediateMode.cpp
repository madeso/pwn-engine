#include "MeshCompiler_ImmediateMode.hpp"
#include <boost/foreach.hpp>
#include <pwn/math/operations.h>
#include "opengl_debug.hpp"

namespace pwn
{
	namespace render
	{
		namespace
		{
			class SharedMesh
			{
			public:
				explicit SharedMesh(const mesh::Mesh& mesh)
					: positions(mesh.data().getCount())
				{
					for(uint32 i=0; i<mesh.data().getCount(); ++i)
					{
						mesh.data().assign(&positions[i], i);
					}
				}

				std::vector<mesh::Point> positions;
			};

			class ImmediateMode : public CompiledMesh
			{
			public:
				ImmediateMode(boost::shared_ptr<SharedMesh> smesh, const mesh::Mesh::TriList& tridata)
					: smesh(smesh)
					, triangles(tridata)
				{
				}

				~ImmediateMode()
				{
				}

				void render(const mesh::CompiledPose& pose)
				{
					pwnAssert_NoGLError();
					glBegin(GL_TRIANGLES);

					BOOST_FOREACH(const mesh::Triangle& tri, triangles)
					{
						for(int i=0; i<3; ++i)
						{
							const mesh::Point& point = smesh->positions[tri[i]];

							const math::vec3 n = point.hasBone()
								? math::GetNormalized(math::cmat33(math::SetTransform(pose.transforms[point.getBone()], math::vec3(0,0,0))) * point.normal)
								: point.normal;

							glNormal3fv(n.data());

							glTexCoord2fv(point.textcoord.data());

							const math::vec3 p = point.hasBone()
								? math::cvec3(pose.transforms[point.getBone()] * math::cvec4(point.location))
								: point.location;
							glVertex3fv(p.data());
						}
					}
					glEnd(); pwnAssert_NoGLError();
				}
			private:
				boost::shared_ptr<SharedMesh> smesh;
				mesh::Mesh::TriList triangles;
			};

			class MeshCompiler_ImmediateMode : public MeshCompiler
			{
			private:
				boost::shared_ptr<SharedMesh> smesh;
			public:
				explicit MeshCompiler_ImmediateMode(const mesh::Mesh& mesh)
					: smesh( new SharedMesh(mesh) )
					{
				}
				~MeshCompiler_ImmediateMode()
				{
				}
				CompiledMesh* compile(const mesh::Mesh::TriList& tridata)
				{
					return new ImmediateMode(smesh, tridata);
				}
			};
		}

		MeshCompiler* CreateMeshCompiler_ImmediateMode(const mesh::Mesh& mesh)
		{
			return new MeshCompiler_ImmediateMode(mesh);
		}
	}
}
