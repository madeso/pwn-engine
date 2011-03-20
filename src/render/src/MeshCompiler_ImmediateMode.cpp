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
						positions[i] = mesh.data().getPoint(i);
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
							math::vec3 normal;
							math::vec3 vertex;

							if( point.hasBone() )
							{
								normal = point.normal;
								vertex = point.location;
							}
							else
							{
								const math::vec4 bone = point.getBone();
								for(int b=0; b<4; ++b)
								{
									const real val = bone[b];
									const mesh::BoneIndex boneIndex = mesh::GetBoneIndex(val);
									const real inf = mesh::GetBoneInfluence(val);
									normal += math::GetNormalized(math::cmat33(math::SetTransform(pose.transforms[boneIndex], math::vec3(0,0,0))) * point.normal);
									vertex += math::cvec3(pose.transforms[boneIndex] * math::cvec4(point.location));
								}

								normal = math::GetNormalized(normal);
							}

							glNormal3fv(normal.data());

							glTexCoord2fv(point.textcoord.data());
							glVertex3fv(vertex.data());
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
