#include <pwn/render/renderlist>
#include <pwn/math/operations>
#include <pwn/render/compiledmesh>
#include <pwn/render/material>

#include <SFML/OpenGl.hpp>

#pragma warning(disable:4512) //'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

namespace pwn
{
	namespace render
	{
		RenderList::RenderList()
		{
		}
		RenderList::~RenderList()
		{
		}

		// for resize(0) support
		RenderList::Command::Command()
			: mat( math::mat44Identity() )
		{
		}

		RenderList::Command::Command(MeshPtr mesh, MaterialPtr material, const math::mat44& mat)
			: mesh(mesh)
			, material(material)
			, mat(mat)
		{
		}

		void RenderList::begin()
		{
			commands.resize(0);
		}

		void RenderList::add(MeshPtr mesh, MaterialPtr material, const math::mat44& mat)
		{
			commands.push_back(Command(mesh, material, mat));
		}

		void RenderList::end()
		{
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			// todo: sort

			BOOST_FOREACH(const Command& c, commands)
			{
				glLoadMatrixf( c.mat.columnMajor );
				//glTranslatef(-1.5f,0.0f,-12.0f);
				apply(c.material);
				c.mesh->render();
			}
		}

		void RenderList::apply(MaterialPtr material)
		{
			glColor4fv( material->diffuse.data() );
			/*
			const GLenum face = GL_FRONT_AND_BACK;
			glMaterialfv(face, GL_AMBIENT, material->ambient.data());
			glMaterialfv(face, GL_DIFFUSE, material->diffuse.data());
			glMaterialfv(face, GL_SPECULAR, material->specular.data());
			glMaterialfv(face, GL_EMISSION, material->emission.data());
			glMaterialf(face, GL_SHININESS, material->shininess);
			*/
		}
	}
}