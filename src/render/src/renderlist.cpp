#include <pwn/render/renderlist.h>
#include <pwn/math/operations.h>
#include <pwn/render/compiledmesh.h>
#include <pwn/render/material.h>
#include "opengl_debug.hpp"
#include <limits>
#include <pwn/render/poseable.h>

#include <pwn/sfml-config.h>
#include <SFML/OpenGL.hpp>

#pragma warning(disable:4512) //'boost::detail::addr_impl_ref<T>' : assignment operator could not be generated
#include <boost/foreach.hpp>

#ifndef MSC_VER
#include <iostream>
namespace
{
    void OutputDebugStringA(const std::string& s)
    {
        std::cout << s << std::endl;
    }
}
#endif

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
		RenderList::DebugCommand::DebugCommand()
			: mat( math::mat44Identity() )
		{
		}

		RenderList::DebugCommand::DebugCommand(const math::mat44& m, Poseable* pos)
			: mat(m)
			, poseable(pos)
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
				const uint64 distance =  CompressDistance(material->hasTransperency, 0, math::Z(math::cvec3(mat)), 100); // todo: use the camera frustrum


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

		RenderList::Command::Command(MeshPtr mesh, MaterialPtr material, const math::mat44& mat, Poseable* pos)
			: mesh(mesh)
			, material(material)
			, mat(mat)
			, poseable(pos)
			, id( CalcId(mesh, material, mat) )
		{
		}

		void RenderList::begin()
		{
			transparent.resize(0);
			solid.resize(0);
			debug.resize(0);
		}

		void RenderList::add(MeshPtr mesh, MaterialPtr material, const math::mat44& mat, Poseable* pos)
		{
			if( material->hasTransperency )
			{
				transparent.push_back(Command(mesh, material, mat, pos));
			}
			else
			{
				solid.push_back(Command(mesh, material, mat, pos));
			}
		}

		void RenderList::add(const math::mat44& mat, Poseable* pos)
		{
			debug.push_back(DebugCommand(mat, pos));
		}

		void RenderList::render(const CommandList& commands, bool applyMaterials)
		{
			BOOST_FOREACH(const RenderList::Command& c, commands)
			{
				if( useGlCommands )
				{
					glLoadMatrixf( c.mat.data()); pwnAssert_NoGLError();
				}
				apply(c.material, applyMaterials);
				c.mesh->render(c.poseable->pose);
			}
		}

		bool CommandSort(const RenderList::Command& lhs, const RenderList::Command& rhs)
		{
			return lhs.id < rhs.id;
		}

		void RenderList::end(bool applyMaterials)
		{
			if( useGlCommands )
			{
				glMatrixMode( GL_MODELVIEW ); pwnAssert_NoGLError();
				glLoadIdentity(); pwnAssert_NoGLError();
			}

			applied = false;
			texture = 0;

			// todo: send correct commands to gl
			if( useGlCommands )
			{
				glAlphaFunc ( GL_GREATER, 0.2f ); pwnAssert_NoGLError();

				glEnable ( GL_ALPHA_TEST ); pwnAssert_NoGLError();
			}

			std::sort(solid.begin(), solid.end(), CommandSort);
			render(solid, applyMaterials);
			std::sort(transparent.begin(), transparent.end(), CommandSort);

			if( useGlCommands )
			{
				glDisable(GL_ALPHA_TEST); pwnAssert_NoGLError();

				glEnable(GL_BLEND); pwnAssert_NoGLError();

				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); pwnAssert_NoGLError();

				// disable depth-write
				glDepthMask(GL_FALSE); pwnAssert_NoGLError();
			}

			render(transparent, applyMaterials);

			if( useGlCommands )
			{
				// enable depth.write again
				glDepthMask(GL_TRUE); pwnAssert_NoGLError();

				glDisable(GL_BLEND); pwnAssert_NoGLError();

				if( applied )
				{
					glDisable(GL_TEXTURE_2D); pwnAssert_NoGLError();
				}

				// before rendering?
				glDisable(GL_LIGHTING);
				glDisable(GL_DEPTH_TEST);

				render(kDebugRenderLines);
				render(kDebugRenderPoints);

				glEnable(GL_LIGHTING);
				glEnable(GL_DEPTH_TEST);
			}
		}

		void Debug_RenderLines(const mesh::CompiledPose& pose)
		{
			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
			glLineWidth(2.0f);
			glBegin(GL_LINES);
			/*for (int i = 0; i < m_model->GetNumJoints(); i++)
			{
				ms3d_joint_t *joint = m_model->GetJoint(i);
				if (joint->parentIndex == -1)
				{
					glVertex3f(joint->matGlobal[0][3], joint->matGlobal[1][3], joint->matGlobal[2][3]);
					glVertex3f(joint->matGlobal[0][3], joint->matGlobal[1][3], joint->matGlobal[2][3]);
				}
				else
				{
					ms3d_joint_t *parentJoint = m_model->GetJoint(joint->parentIndex);
					glVertex3f(joint->matGlobal[0][3], joint->matGlobal[1][3], joint->matGlobal[2][3]);
					glVertex3f(parentJoint->matGlobal[0][3], parentJoint->matGlobal[1][3], parentJoint->matGlobal[2][3]);
				}
			}*/
			glEnd();
		}

		void Debug_RenderPoints(const mesh::CompiledPose& pose)
		{
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			BOOST_FOREACH(const math::mat44& m, pose.transforms)
			{
				glVertex3f(m(0, 3), m(1, 3), m(2, 3));
			}
			glEnd();
		}

		void RenderList::render(DebugRenderType rt)
		{
			BOOST_FOREACH(const DebugCommand& c, debug)
			{
				if( useGlCommands )
				{
					glLoadMatrixf( c.mat.data()); pwnAssert_NoGLError();
				}
				if( rt == kDebugRenderLines )
				{
					Debug_RenderLines(c.poseable->pose);
				}
				else if( rt == kDebugRenderPoints )
				{
					Debug_RenderPoints(c.poseable->pose);
				}
				else
				{
					Assert(false);
				}
			}
		}

		void RenderList::apply(MaterialPtr material, bool applyMaterials)
		{
			if( useGlCommands )
			{
				if( applyMaterials )
				{
					const GLenum face = GL_FRONT;
					glMaterialfv(face, GL_AMBIENT, material->ambient.data()); pwnAssert_NoGLError();
					glMaterialfv(face, GL_DIFFUSE, material->diffuse.data()); pwnAssert_NoGLError();
					glMaterialfv(face, GL_SPECULAR, material->specular.data()); pwnAssert_NoGLError();
					glMaterialfv(face, GL_EMISSION, material->emission.data()); pwnAssert_NoGLError();
					glMaterialf(face, GL_SHININESS, material->shininess); pwnAssert_NoGLError();
				}
				else
				{
					glColor4f( material->diffuse.red(), material->diffuse.green(), material->diffuse.blue(), material->diffuse.alpha() );
					pwnAssert_NoGLError();
				}
				if( material->texture.get() )
				{
					if( applied == false )
					{
						glEnable(GL_TEXTURE_2D); pwnAssert_NoGLError();
					}
					if( material->texture.get() != texture )
					{
						material->texture->bind(0);
					}
					applied = true;
					texture = material->texture.get();
				}
				else
				{
					if( applied )
					{
						glDisable(GL_TEXTURE_2D); pwnAssert_NoGLError();
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
