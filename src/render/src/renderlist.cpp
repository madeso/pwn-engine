#include <pwn/render/renderlist.h>
#include <pwn/math/operations.h>
#include <pwn/render/compiledmesh.h>
#include <pwn/render/material.h>
#include <pwn/assert.h>
#include <limits>

#include <SFML/OpenGl.hpp>
#pragma comment(lib, "opengl32.lib")

#pragma warning(disable:4512) //'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

namespace pwn
{
	namespace render
	{
		
		GLenum glGetError_WithString()
		{
			const GLenum err = glGetError();
			switch(err)
			{
			case GL_NO_ERROR:
				break;
#define PRINT(x) OutputDebugStringA("--> Pwn detected a open gl error: " x"!\n")
#define ERR(x) case x: PRINT(#x); break;
			ERR(GL_INVALID_ENUM)
			ERR(GL_INVALID_VALUE)
			ERR(GL_INVALID_OPERATION)
			ERR(GL_STACK_OVERFLOW)
			ERR(GL_STACK_UNDERFLOW)
			ERR(GL_OUT_OF_MEMORY)
//				ERR(GL_TABLE_TOO_LARGE)
			default:
				PRINT("UNKNOWN");

#undef PRINT
#undef ERR
			}
			return err;
		}
		

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
			const uint32 CompressDistance(bool reverse, const real znear, const real dist, const real zfar)
			{
				const real value = pwn::math::KeepWithin(0, pwn::math::To01(znear, dist, zfar), 1);
				const real v = reverse ? 1-value : value;
				return static_cast<uint32>(v * std::numeric_limits<uint32>::max());
			}

			RenderList::ID CalcId(RenderList::MeshPtr, RenderList::MaterialPtr material, const math::mat44& mat)
			{
				// todo: calculate a better id
				const uint64 texture = material->texture ? material->texture->sid() : 0;
				const uint64 distance =  CompressDistance(material->hasTransperency, 0, cvec3(mat).z, 100); // todo: use the camera frustrum


				if( material->hasTransperency )
				{
					return (distance << 32) | texture;
				}
				else
				{
					return (texture << 32) | distance;
				}
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
					Assert( glGetError_WithString() == GL_NO_ERROR);
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
				Assert( glGetError_WithString() == GL_NO_ERROR);
				glLoadIdentity();
				Assert( glGetError_WithString() == GL_NO_ERROR);
			}
			
			applied = false;
			texture = 0;
			
			// todo: send correct commands to gl
			if( useGlCommands )
			{
				glAlphaFunc ( GL_GREATER, 0.2f ) ;
				Assert( glGetError_WithString() == GL_NO_ERROR);

				glEnable ( GL_ALPHA_TEST ) ;
				Assert( glGetError_WithString() == GL_NO_ERROR);
			}

			std::sort(solid.begin(), solid.end(), CommandSort);
			render(solid);
			std::sort(transparent.begin(), transparent.end(), CommandSort);

			if( useGlCommands )
			{
				glDisable(GL_ALPHA_TEST);
				Assert( glGetError_WithString() == GL_NO_ERROR);

				glEnable(GL_BLEND);
				Assert( glGetError_WithString() == GL_NO_ERROR);

				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				Assert( glGetError_WithString() == GL_NO_ERROR);

				glDepthMask(GL_FALSE); // disable depth-write
				Assert( glGetError_WithString() == GL_NO_ERROR);
			}

			render(transparent);

			if( useGlCommands )
			{
				glDepthMask(GL_TRUE); // enable depth.write again
				Assert( glGetError_WithString() == GL_NO_ERROR);

				glDisable(GL_BLEND);
				Assert( glGetError_WithString() == GL_NO_ERROR);

				if( applied )
				{
					glDisable(GL_TEXTURE_2D);
					Assert( glGetError_WithString() == GL_NO_ERROR);
				}
			}
		}

		void RenderList::apply(MaterialPtr material)
		{
			if( useGlCommands )
			{
				glColor4f( material->diffuse.red(), material->diffuse.green(), material->diffuse.blue(), material->diffuse.alpha() );
				Assert( glGetError_WithString() == GL_NO_ERROR);
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