#include "MilkshapeCommon.hpp"

#include <boost/foreach.hpp>
#include <pwn/math/operations.h>
#include <pwn/core/StringUtils.h>
#include <boost/filesystem.hpp>
#include <pwn/mesh/mesh.h>
#include <pwn/mesh/material.h>
#include "Converter.hpp"
#include <boost/lexical_cast.hpp>

namespace pwn
{
	namespace convert
	{
		namespace milkshape
		{
			namespace common
			{
				Model::Model()
					: framecount(0)
					, currentFrame(0)
				{
				}

				Mesh& Model::newMesh()
				{
					meshes.push_back(Mesh());
					return *meshes.rbegin();
				}

				Material& Model::newMaterial()
				{
					materials.push_back(Material());
					return *materials.rbegin();
				}

				Bone& Model::newBone()
				{
					bones.push_back(Bone());
					return *bones.rbegin();
				}

				int Model::boneId(string bone)
				{
					if (bone == "") return -1;
					for (std::size_t i = 0; i < bones.size(); ++i)
					{
						if (bones[i].name == bone) return i;
					}
					throw "Failed to find a match for " + bone;
				}

				void Model::mapBonesToId()
				{
					BOOST_FOREACH(Bone& b, bones)
					{
						b.parentId = boneId(b.parentName);
					}
				}

			
				Mesh::Mesh()
					: flags(0)
					, materialId(0)
				{
				}

				Vertex& Mesh::newVertex()
				{
					vertices.push_back(Vertex());
					return *vertices.rbegin();
				}

				Normal& Mesh::newNormal()
				{
					normals.push_back(Normal());
					return *normals.rbegin();
				}

				Tri& Mesh::newTri()
				{
					tris.push_back(Tri());
					return *tris.rbegin();
				}

				Vertex::Vertex()
					: flags(0)
					, uv(0,0)
					, pos(0,0,0)
					, bone(0)
				{
				}

				/*string ToString()
				{
					return string.Format("{0} {1} / {2}",bone, pos, uv);
				}*/

				Normal::Normal()
					: norm(0,0,0)
				{
				}

				void Normal::normalize()
				{
					Normalize(&norm);
				}

				Tri::Tri()
					: flags(0)
					, v1(0), v2(0), v3(0)
					, n1(0), n2(0), n3(0)
					, smoothingGroup(0)
				{
				}

				void Tri::buildNormal(Mesh mesh)
				{
					// not needed?
				}

				Color::Color()
					: r(1)
					, g(1)
					, b(1)
					, a(1)
				{
				}

				void Color::parse(string p)
				{
					pwn::istringstream ss(p);
					ss >> r;
					ss >> g;
					ss >> b;
					ss >> a;
				}

			

				Material::Material()
					: name("")
					, shininess(0)
					, transperency(1)
					, diffuseTexture("")
					, alphatexture("")
				{
				}
			

			

				/*string RotationKey::ToString()
				{
					return string.Format("{0} {1}", time, MilkshapeCommon.makeQuat( math::vec3(x, y, z)).AxisAngle);
				}*/
			

			
				Bone::Bone()
					: name("")
					, parentName("")
					, parentId(-1)
					, flags(0)
				{
				}

				/*string Bone::ToString()
				{
					return name + ": " + parentName;
				}*/

				RotationKey& Bone::newRotationKey()
				{
					rotations.push_back(RotationKey());
					return *rotations.rbegin();
				}

				PositionKey& Bone::newPositionKey()
				{
					positions.push_back(PositionKey());
					return *positions.rbegin();
				}

				namespace MilkshapeCommon
				{
					/*Animation ExtractAnimation(Model model)
					{
						std::vector<AnimationForBone> ani =  std::vector<AnimationForBone>();

						bool added = false;

						BOOST_FOREACH (Bone b , model.bones)
						{
							AnimationForBone a =  AnimationForBone();

							BOOST_FOREACH (PositionKey pk , b.PositionKeys)
							{
								a.addPositon(pk.time,  math::vec3(pk.x, pk.y, pk.z));
								added = true;
							}
							BOOST_FOREACH (RotationKey rk , b.RotationKeys)
							{
								a.addRotation(rk.time, makeQuat( math::vec3(rk.x, rk.y, rk.z)));
								added = true;
							}

							ani.push_back(a);
						}

						if (added) return  Animation(ani);
						else return null;
					}*/

					void ExtractMeshDefinition(const Model& model, OptimizedMeshBuilder* builder)
					{
						int id = 0;
						BOOST_FOREACH(const Material& mat, model.materials)
						{
							mesh::Mesh::MaterialPtr smat( new mesh::Material() );
							smat->texture_diffuse = mat.diffuseTexture;
							builder->addMaterial(smat);
							++id;
						}

						/*BOOST_FOREACH (Bone b , model.bones)
						{
							MeshDef.Bone bn = def.newBone();
							bn.pos =  math::vec3(b.x, b.y, b.z);
							bn.rot = makeQuat( math::vec3(b.rx, b.ry, b.rz));
							bn.parent = b.parentId;
							bn.name = b.name;
						}*/

						int vadded = 0;
						int vbase = 0;
						int nadded = 0;
						int nbase = 0;
						BOOST_FOREACH (Mesh me , model.meshes)
						{
							vbase += vadded;
							vadded = 0;
							nbase += nadded;
							nadded = 0;

							//def.selectMaterial("m" + me.materialId);
							BOOST_FOREACH (Vertex v , me.vertices)
							{
								builder->addPosition(v.pos/*, v.bone*/);
								builder->addTextCoord(math::vec2(v.uv.x, 1 - v.uv.y));
								++vadded;
							}

							BOOST_FOREACH (Normal n , me.normals)
							{
								builder->addNormal(n.norm);
								++nadded;
							}

							BOOST_FOREACH (Tri tr , me.tris)
							{
								builder->addTriangle(mesh::Triangle(me.materialId
									, mesh::Triangle::Vertex(vbase + tr.v1, nbase + tr.n1)
									, mesh::Triangle::Vertex(vbase + tr.v2, nbase + tr.n2)
									, mesh::Triangle::Vertex(vbase + tr.v3, nbase + tr.n3)
									));
							}
						}
						builder->done();
					}

					math::quat makeQuat(math::vec3 angles)
					{
						real ang;
						real sr, sp, sy, cr, cp, cy;

						// FIXME: rescale the inputs to 1/2 angle
						real ascale = 0.5f;
						ang = angles[2] * ascale;
						sy = math::Sin( math::Angle::FromRadians(ang));
						cy = math::Cos( math::Angle::FromRadians(ang));
						ang = angles[1] * ascale;
						sp = math::Sin( math::Angle::FromRadians(ang));
						cp = math::Cos( math::Angle::FromRadians(ang));
						ang = angles[0] * ascale;
						sr = math::Sin( math::Angle::FromRadians(ang));
						cr = math::Cos( math::Angle::FromRadians(ang));

						real x = sr * cp * cy - cr * sp * sy; // X
						real y = cr * sp * cy + sr * cp * sy; // Y
						real z = cr * cp * sy - sr * sp * cy; // Z
						real w = cr * cp * cy + sr * sp * sy; // W
						math::quat a =  math::GetNormalized(math::quat(w,  math::vec3(x, y, z)));
						return a;
					}
				}
			}
		}
	}
}