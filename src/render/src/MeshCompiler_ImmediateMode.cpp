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
					for(uint32 i = 0; i < mesh.data().getCount(); ++i)
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

					BOOST_FOREACH(const mesh::Triangle & tri, triangles)
					{
						for(int i = 0; i < 3; ++i)
						{
							const mesh::Point& point = smesh->positions[tri[i]];
							math::vec3 normal(0, 0, 0);
							math::vec3 vertex(0, 0, 0);

							if(false == point.hasBone())
							{
								normal = point.normal;
								vertex = point.position;
							}
							else
							{
								const math::vec4 bone = point.getBone();
								real infsum = 0;
								for(int b = 0; b < 4; ++b)
								{
									const real val = bone[b];
									if(val < 0.0f)
									{
										break;
									}
									const real inf = mesh::GetBoneInfluence(val);
									const mesh::BoneIndex boneIndex = mesh::GetBoneIndex(val);
									infsum += inf;
									normal += math::GetNormalized(math::cmat33(math::SetTransform(pose.transforms[boneIndex].first, math::vec3(0, 0, 0))) * point.normal);
									vertex += math::cvec3(pose.transforms[boneIndex].first * math::cvec4(point.position));
								}
								Assert(infsum > 0);
								const real inv = 1 / infsum;
								vertex *= inv;
								normal = math::GetNormalized(math::vec3(inv * normal));
							}

							glNormal3fv(normal.data());

							glTexCoord2fv(point.textcoord.data());
							glVertex3fv(vertex.data());
						}
					}
					glEnd();
					pwnAssert_NoGLError();
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
					: smesh(new SharedMesh(mesh))
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
