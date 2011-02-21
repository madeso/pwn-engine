#include <pwn/render/actordef.h>
#include <pwn/mesh/mesh.h>
#include <pwn/render/part.h>
#pragma warning(disable:4512) //'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>
#include <pwn/render/compiledmesh.h>
#include <pwn/render/material.h>
#include <pwn/render/texturepool2.h>
#include <pwn/math/operations.h>

#include <pwn/assert.h>

#include "opengl_debug.hpp"

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
			{
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
							? math::cmat33(pose.transforms[point.getBone()]) * point.location
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

		boost::shared_ptr<render::Material> Compile(const mesh::Material mm, TexturePool2* pool)
		{
			boost::shared_ptr<render::Material> rm( new render::Material() );

			// todo: move to the render::Material constructor..
			rm->ambient = mm.ambient;
			rm->diffuse = mm.diffuse;
			rm->specular = mm.specular;
			rm->emission = mm.emission;
			rm->shininess = mm.shininess;

			// todo: check more than diffuse alpha
			rm->hasTransperency = mm.diffuse.alpha() < 0.9f;

			if( false == mm.getTexture_Diffuse().empty() )
			{
				rm->texture = pool->get(mm.texture_diffuse);
			}
			return rm;
		}

		boost::shared_ptr<ActorDef> Compile(const mesh::Mesh& mesh, TexturePool2* pool)
		{
			boost::shared_ptr<ActorDef> def( new ActorDef() );
			boost::shared_ptr<SharedMesh> smesh( new SharedMesh(mesh) );

			// todo: implement better rendering
			BOOST_FOREACH(mesh::Mesh::TriangleMap::const_reference r, mesh.triangles)
			{
				ActorDef::PartPtr part( new Part() );
				const pwn::uint32 index = r.first;
				part->material = Compile( mesh.materials[index], pool );
				part->mesh.reset( new ImmediateMode(smesh, r.second) );
				def->parts.push_back(part);
			}

			def->bones = mesh.bones;
			return def;
		}
	}
}
