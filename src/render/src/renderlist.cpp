#include <pwn/render/renderlist>
#include <pwn/math/operations>
#include <pwn/render/compiledmesh>
#include <pwn/render/material>
#include <pwn/assert>

#include <SFML/OpenGl.hpp>
#pragma comment(lib, "opengl32.lib")

#pragma warning(disable:4512) //'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

namespace pwn
{
	namespace render
	{
		RenderList::RenderList(bool useGlCommands)
			: useGlCommands(useGlCommands)
			, texture(0)
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

		void RenderList::render(const CommandList& commands)
		{
			BOOST_FOREACH(const RenderList::Command& c, commands)
			{
				if( useGlCommands )
				{
					glLoadMatrixf( c.mat.columnMajor );
					Assert( glGetError() == GL_NO_ERROR);
				}
				apply(c.material);
				c.mesh->render();
			}
		}

		bool CommandSort(const RenderList::Command& lhs, const RenderList::Command& rhs)
		{
			return lhs.id < rhs.id;
		}

		void RenderList::end()
		{
			if( useGlCommands )
			{
				glMatrixMode( GL_MODELVIEW );
				Assert( glGetError() == GL_NO_ERROR);
				glLoadIdentity();
				Assert( glGetError() == GL_NO_ERROR);
			}
			
			applied = false;
			texture = 0;
			
			// todo: send correct commands to gl
			if( useGlCommands )
			{
				glAlphaFunc ( GL_GREATER, 0.2f ) ;
				Assert( glGetError() == GL_NO_ERROR);

				glEnable ( GL_ALPHA_TEST ) ;
				Assert( glGetError() == GL_NO_ERROR);
			}

			std::sort(solid.begin(), solid.end(), CommandSort);
			render(solid);
			std::sort(transparent.begin(), transparent.end(), CommandSort);

			if( useGlCommands )
			{
				glDisable(GL_ALPHA_TEST);
				Assert( glGetError() == GL_NO_ERROR);

				glEnable(GL_BLEND);
				Assert( glGetError() == GL_NO_ERROR);

				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				Assert( glGetError() == GL_NO_ERROR);

				glDepthMask(GL_FALSE); // disable depth-write
				Assert( glGetError() == GL_NO_ERROR);
			}

			render(transparent);

			if( useGlCommands )
			{
				glDepthMask(GL_TRUE); // enable depth.write again
				Assert( glGetError() == GL_NO_ERROR);

				glDisable(GL_BLEND);
				Assert( glGetError() == GL_NO_ERROR);

				if( applied )
				{
					glDisable(GL_TEXTURE_2D);
					Assert( glGetError() == GL_NO_ERROR);
				}
			}
		}

		void RenderList::apply(MaterialPtr material)
		{
			if( useGlCommands )
			{
				glColor4fv( material->diffuse.data() );
				Assert( glGetError() == GL_NO_ERROR);
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
}