#include <pwn/string.h>
#include <fstream>
#include <boost/scoped_array.hpp>
#include <boost/foreach.hpp>
#include "MilkshapeCommon.hpp"
#include <pwn/mesh/mesh.h>
#include "MilkshapeBinary.hpp"

namespace pwn
{
	namespace convert
	{
		namespace milkshape
		{
			namespace binary
			{
				using namespace milkshape::common;

				typedef unsigned char byte;

				class MeshGroup
				{
				public:
					string name;
					int material;
					std::vector<int> tri;
				};

				#pragma pack(push,1)
				struct MS3DHeader
				{
					char ID[10];
					int version;
				};
				struct MS3DVertex
				{
					byte flags;
					float vertex[3];
					char boneID;
					byte refCount;
				};

				struct MS3DTriangle
				{
					unsigned short flags;
					unsigned short vertexIndices[3];
					float vertexNormals[3][3]; //[3],[3]
					float s[3];
					float t[3];
					byte smoothingGroup;
					byte groupIndex;
				};
				struct MS3DMaterial
				{
					char name[32]; //
					float ambient[4]; //
					float diffuse[4]; //
					float specular[4]; //
					float emissive[4]; //
					float shininess; // 0.0f - 128.0f
					float transparency; // 0.0f - 1.0f
					char mode; // 0, 1, 2 is unused now
					char texture[128];
					char alphamap[128];
	/*
					char name[32];
					float ambient;
					float diffuse[4];
					float specular[4];
					float emissive[4];
					float shininess; // 0.0f - 128.0f
					float transparency; // 0.0f - 1.0f
					char mode; // 0, 1, 2 is unused now
					char texture[128];
					char alphamap[128];
					*/
				};
				struct MS3DJoint
				{
					byte flags;
					char name[32];
					char parentName[32];
					float rotation[3];
					float translation[3];
					unsigned short numRotationKeyframes;
					unsigned short numTranslationKeyframes;
				};
				struct MS3DKeyframe
				{
					float time;
					float parameter[3];
				};
				#pragma pack(pop)

				void copyToColor(Color& color,float* p)
				{
					color.r = p[0];
					color.g = p[1];
					color.b = p[2];
					color.a = p[3];
				}
				void copyToColor(Color& color,float p)
				{
					color.r = p;
					color.g = p;
					color.b = p;
					color.a = p;
				}

				class Joint
				{
				public:
					MS3DJoint joint;
					std::vector<MS3DKeyframe> rotations;
					std::vector<MS3DKeyframe> translations;
				};

				class Runner
				{
					std::ifstream fs;

				public:
					template<typename T>
					T Read()
					{
						T t;
						fs.read( reinterpret_cast<char*>(&t), sizeof(T));
						if( fs.good() == false) throw "bad file";
						return t;
					}

					int ReadCount()
					{
						return Read<unsigned short>();
					}

					string ReadString(int count)
					{
						boost::scoped_array<char> str( new char[count] );
						fs.read(str.get(), count);
						return str.get();
					}

					Runner(const std::string& path)
						: fs(path.c_str(), std::ios::binary)
					{
						if( fs.good() == false ) throw "failed to open " + path;
					}

					void run()
					{
						MS3DHeader header = Read<MS3DHeader>();
						readVertices();
						readTriangles();
						readMeshGroups();
						readMaterials();
						animfps = Read<float>();
						current = Read<float>();
						framecount = Read<int>();
						readJoints();
					}

					void readJoints()
					{
						int jointcount = ReadCount();

						for (int ji = 0; ji < jointcount; ++ji)
						{
							Joint joint;
							joint.joint = Read<MS3DJoint>();
							for (int ki = 0; ki < joint.joint.numRotationKeyframes; ++ki)
							{
								MS3DKeyframe kf = Read<MS3DKeyframe>();
								joint.rotations.push_back(kf);
							}
							for (int ki = 0; ki < joint.joint.numTranslationKeyframes; ++ki)
							{
								MS3DKeyframe kf = Read<MS3DKeyframe>();
								joint.translations.push_back(kf);
							}

							joints.push_back(joint);
						}
					}

					void readMaterials()
					{
						int matcount = ReadCount();
						for (int m = 0; m < matcount; ++m)
						{
							MS3DMaterial mat = Read<MS3DMaterial>();
							materials.push_back(mat);
						}
					}

					void readMeshGroups()
					{
						int meshgroups = ReadCount();
						for (int m = 0; m < meshgroups; ++m)
						{
							MeshGroup group;
							byte flags = Read<byte>();
							group.name = ReadString(32);
							int mtricount = ReadCount();
							for (int t = 0; t < mtricount; ++t)
							{
								int tr = Read<short>();
								group.tri.push_back(tr);
							}
							group.material = Read<char>();
							groups.push_back(group);
						}
					}

					void readTriangles()
					{
						int tricount = ReadCount();
						for (int t = 0; t < tricount; ++t)
						{
							MS3DTriangle tri = Read<MS3DTriangle>();
							triangles.push_back(tri);
						}
					}

					void readVertices()
					{
						int vertcount = ReadCount();
						for (int v = 0; v < vertcount; ++v)
						{
							MS3DVertex vt = Read<MS3DVertex>();
							vertices.push_back(vt);
						}
					}

					// result
					int framecount;
					float current;
					float animfps;

					std::vector<MS3DVertex> vertices;
					std::vector<MS3DTriangle> triangles;
					std::vector<MS3DMaterial> materials;
					std::vector<MeshGroup> groups;
					std::vector<Joint> joints;
				};

				Model ExtractModel(Runner& run)
				{
					Model model;
					model.framecount = run.framecount;
					model.currentFrame = run.current;

					BOOST_FOREACH(MS3DMaterial s, run.materials)
					{
						Material& mat = model.newMaterial();
						mat.name = s.name;
						copyToColor(mat.ambient, s.ambient);
						copyToColor(mat.diffuse, s.diffuse);
						copyToColor(mat.specular, s.specular);
						copyToColor(mat.emissive, s.emissive);
						mat.shininess = s.shininess;
						mat.transperency = s.transparency;
						mat.diffuseTexture = s.texture;
						mat.alphatexture = s.alphamap;
					}

					BOOST_FOREACH(MeshGroup g, run.groups)
					{
						Mesh& m = model.newMesh();
						m.materialId = g.material;
						BOOST_FOREACH(int tr, g.tri)
						{
							MS3DTriangle& tri = run.triangles[tr];
							int vid[3];
							int nid[3];

							for (int i = 0; i < 3; ++i)
							{
								vid[i] = m.vertices.size();
								nid[i] = m.normals.size();

								Normal& norm = m.newNormal();
								math::X(norm.norm) = tri.vertexNormals[i][0];
								math::Y(norm.norm) = tri.vertexNormals[i][1];
								math::Z(norm.norm) = tri.vertexNormals[i][2];

								MS3DVertex v = run.vertices[ tri.vertexIndices[i] ];
								Vertex& ver = m.newVertex();
								math::X(ver.uv) = tri.s[i];
								math::Y(ver.uv) = tri.t[i];
								ver.bone = v.boneID;
								math::X(ver.pos) = v.vertex[0];
								math::Y(ver.pos) = v.vertex[1];
								math::Z(ver.pos) = v.vertex[2];
							}

							Tri& t = m.newTri();
							t.n1 = nid[0];
							t.n2 = nid[1];
							t.n3 = nid[2];
							t.v1 = vid[0];
							t.v2 = vid[1];
							t.v3 = vid[2];
						}
					}

					BOOST_FOREACH(Joint j, run.joints)
					{
						Bone& b = model.newBone();
						b.parentName = j.joint.parentName;
						b.name = j.joint.name;
						b.rx = j.joint.rotation[0];
						b.ry = j.joint.rotation[1];
						b.rz = j.joint.rotation[2];
						b.x = j.joint.translation[0];
						b.y = j.joint.translation[1];
						b.z = j.joint.translation[2];

						BOOST_FOREACH(MS3DKeyframe r, j.rotations)
						{
							RotationKey& k = b.newRotationKey();
							k.time = r.time * run.animfps;
							k.x = r.parameter[0];
							k.y = r.parameter[1];
							k.z = r.parameter[2];
						}

						BOOST_FOREACH(MS3DKeyframe t, j.translations)
						{
							PositionKey& k = b.newPositionKey();
							k.time = t.time * run.animfps;
							k.x = t.parameter[0];
							k.y = t.parameter[1];
							k.z = t.parameter[2];
						}
					}

					model.mapBonesToId();
					return model;
				}

				void Read(BuilderList* builders, const string& meshpath)
				{
					Model model;

					Runner run(meshpath);
					run.run();
					model = ExtractModel(run);

					mesh::Builder builder;
					MilkshapeCommon::ExtractMeshDefinition(model, &builder);
					Entry e(builder);
					e.animations.push_back(MilkshapeCommon::ExtractAnimation(model));
					builders->push_back(e);
				}
			}
		}
	}
}
