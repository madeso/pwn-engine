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

#include"MeshCompiler.hpp"
#include "MeshCompiler_ImmediateMode.hpp"

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

		boost::shared_ptr<render::Material> Compile(const mesh::Material mm, TexturePool2* pool)
		{
			boost::shared_ptr<render::Material> rm(new render::Material());

			// todo: move to the render::Material constructor..
			rm->ambient = mm.ambient;
			rm->diffuse = mm.diffuse;
			rm->specular = mm.specular;
			rm->emission = mm.emission;
			rm->shininess = mm.shininess;

			// todo: check more than diffuse alpha
			rm->hasTransperency = mm.diffuse.alpha() < 0.9f;

			if(false == mm.getTexture_Diffuse().empty())
			{
				rm->texture = pool->get(mm.texture_diffuse);
			}
			return rm;
		}

		boost::shared_ptr<ActorDef> Compile(const mesh::Mesh& mesh, TexturePool2* pool)
		{
			boost::shared_ptr<ActorDef> def(new ActorDef());
			std::auto_ptr<MeshCompiler> compiler(CreateMeshCompiler_ImmediateMode(mesh));

			// todo: implement better rendering
			BOOST_FOREACH(mesh::Mesh::TriangleMap::const_reference r, mesh.getTriangles())
			{
				ActorDef::PartPtr part(new Part());
				const pwn::uint32 index = r.first;
				part->material = Compile(mesh.getMaterials()[index], pool);
				part->mesh.reset(compiler->compile(r.second));
				def->parts.push_back(part);
			}

			def->bones = mesh.getBones();
			return def;
		}
	}
}
