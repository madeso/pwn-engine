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
			: texture(0)
			, applied(false)
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

		namespace
		{
			RenderList::ID CalcId(RenderList::MeshPtr, RenderList::MaterialPtr, const math::mat44&)
			{
				// todo: calculate a better id
				return 0;
			}
		}

		RenderList::Command::Command(MeshPtr mesh, MaterialPtr material, const math::mat44& mat)
			: mesh(mesh)
			, material(material)
			, mat(mat)
			, id( CalcId(mesh, material, mat) )
		{
		}

		void RenderList::begin()
		{
			transparent.resize(0);
			solid.resize(0);
		}

		void RenderList::add(MeshPtr mesh, MaterialPtr material, const math::mat44& mat)
		{
			if( material->hasTransperency )
			{
				transparent.push_back(Command(mesh, material, mat));
			}
			else
			{
				solid.push_back(Command(mesh, material, mat));
			}
		}

		void Render(RenderList* r, const RenderList::CommandList& commands)
		{
			BOOST_FOREACH(const RenderList::Command& c, commands)
			{
				glLoadMatrixf( c.mat.columnMajor );
				assert( glGetError() == GL_NO_ERROR);
				r->apply(c.material);
				c.mesh->render();
			}
		}

		bool CommandSort(const RenderList::Command& lhs, const RenderList::Command& rhs)
		{
			return lhs.id < rhs.id;
		}

		void RenderList::end()
		{
			glMatrixMode( GL_MODELVIEW );
			assert( glGetError() == GL_NO_ERROR);
			glLoadIdentity();
			assert( glGetError() == GL_NO_ERROR);
			
			applied = false;
			texture = 0;
			
			// todo: send correct commands to gl
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			std::sort(solid.begin(), solid.end(), CommandSort);
			Render(this, solid);
			std::sort(transparent.begin(), transparent.end(), CommandSort);
			glDisable(GL_DEPTH_TEST);
			Render(this, transparent);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);

			if( applied )
			{
				glDisable(GL_TEXTURE_2D);
			}
		}

		void RenderList::apply(MaterialPtr material)
		{
			glColor4fv( material->diffuse.data() );
			assert( glGetError() == GL_NO_ERROR);
			if( material->texture.get() )
			{
				if( applied == false )
				{
					glEnable(GL_TEXTURE_2D);
				}
				if( material->texture.get() != texture )
				{
					glBindTexture(GL_TEXTURE_2D, material->texture->tid());
				}
				applied = true;
				texture = material->texture.get();
			}
			else
			{
				if( applied )
				{
					glDisable(GL_TEXTURE_2D);
				}
				applied = false;
			}
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