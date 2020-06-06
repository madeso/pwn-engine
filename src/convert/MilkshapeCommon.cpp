#include "MilkshapeCommon.hpp"

#include <boost/foreach.hpp>
#include <pwn/math/operations.h>
#include <pwn/core/stringutils.h>
#include <boost/filesystem.hpp>
#include <pwn/mesh/mesh.h>
#include <boost/lexical_cast.hpp>
#include <pwn/mesh/builder.h>

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
					if(bone == "")
					{
						return -1;
					}
					for(unsigned int i = 0; i < bones.size(); ++i)
					{
						if(bones[i].name == bone)
						{
							return i;
						}
					}
					throw "Failed to find a match for " + bone;
				}

				void Model::mapBonesToId()
				{
					BOOST_FOREACH(Bone & b, bones)
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
					, uv(0, 0)
					, pos(0, 0, 0)
					, bone(0)
				{
				}

				/*string ToString()
				{
					return string.Format("{0} {1} / {2}",bone, pos, uv);
				}*/

				Normal::Normal()
					: norm(0, 0, 0)
				{
				}

				void Normal::normalize()
				{
					norm = norm.normalize();
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
					mesh::Animation ExtractAnimation(const Model& model)
					{
						using namespace pwn::mesh;
						std::vector<AnimationPerBone> ani =  std::vector<AnimationPerBone>();

						bool added = false;

						BOOST_FOREACH(const milkshape::common::Bone & b , model.bones)
						{
							AnimationPerBone a =  AnimationPerBone();

							// -1 = milkshape animations start at 1, pwn start at 0
							BOOST_FOREACH(const PositionKey & pk , b.positions)
							{
								a.addPosition(pk.time - 1,  math::vec3(pk.x, pk.y, pk.z));
								added = true;
							}
							BOOST_FOREACH(const RotationKey & rk , b.rotations)
							{
								a.addRotation(rk.time - 1, makeQuat(math::vec3(rk.x, rk.y, rk.z)));
								added = true;
							}

							ani.push_back(a);
						}

						if(added)
						{
							return  Animation(ani);
						}
						else
						{
							throw "no animations found";
						}
					}

					void ExtractMeshDefinition(const Model& model, pwn::mesh::Builder* builder)
					{
						int id = 0;
						BOOST_FOREACH(const Material & mat, model.materials)
						{
							mesh::Material smat;
							smat.setTexture_Diffuse(mat.diffuseTexture);
							builder->addMaterial(mat.name, smat);
							++id;
						}

						BOOST_FOREACH(Bone b , model.bones)
						{
							mesh::Bone bn;
							bn.pos =  math::vec3(b.x, b.y, b.z);
							bn.rot = makeQuat(math::vec3(b.rx, b.ry, b.rz));
							if(b.parentId == -1)
							{
								// bone defaults to no parent, dont do anything
							}
							else
							{
								bn.setParent(b.parentId);
							}
							bn.setName(b.name);
							builder->addBone(bn);
						}

						int vadded = 0;
						int vbase = 0;
						int nadded = 0;
						int nbase = 0;
						BOOST_FOREACH(Mesh me , model.meshes)
						{
							vbase += vadded;
							vadded = 0;
							nbase += nadded;
							nadded = 0;

							//def.selectMaterial("m" + me.materialId);
							BOOST_FOREACH(Vertex v , me.vertices)
							{
								const math::vec4 boneIndex(v.bone + 0.5f, -1, -1, -1);
								builder->addPosition(pwn::mesh::BPoint(v.pos, boneIndex));
								builder->addTextCoord(math::vec2(math::X(v.uv), math::Y(v.uv)));
								++vadded;
							}

							BOOST_FOREACH(Normal n , me.normals)
							{
								builder->addNormal(n.norm);
								++nadded;
							}

							BOOST_FOREACH(Tri tr , me.tris)
							{
								builder->addTriangle(me.materialId, mesh::BTriangle(
								                        mesh::BTriangle::Vertex::Create_VN_T(vbase + tr.v1, nbase + tr.n1)
								                        , mesh::BTriangle::Vertex::Create_VN_T(vbase + tr.v2, nbase + tr.n2)
								                        , mesh::BTriangle::Vertex::Create_VN_T(vbase + tr.v3, nbase + tr.n3)
								                     ));
							}
						}
					}

					math::quat makeQuat(const math::vec3& angles)
					{
						real ang;

						// FIXME: rescale the inputs to 1/2 angle
						const real ascale = 0.5f;
						ang = angles[2] * ascale;
						const real sy = math::Sin(math::Angle::FromRadians(ang));
						const real cy = math::Cos(math::Angle::FromRadians(ang));
						ang = angles[1] * ascale;
						const real sp = math::Sin(math::Angle::FromRadians(ang));
						const real cp = math::Cos(math::Angle::FromRadians(ang));
						ang = angles[0] * ascale;
						const real sr = math::Sin(math::Angle::FromRadians(ang));
						const real cr = math::Cos(math::Angle::FromRadians(ang));

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
