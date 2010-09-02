#include "3ds.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <pwn/string.h>
#include <pwn/number.h>
#include <fstream>
#include <pwn/math/types.h>
#include <pwn/math/operations.h>
#include <pwn/mesh/mesh.h>
#include <pwn/mesh/material.h>
#include <pwn/mesh/builder.h>

#include "Converter.hpp"

namespace pwn
{
	namespace convert
	{
		namespace studio3ds
		{
			namespace hidden
			{
				namespace 
				{
					using namespace pwn::math;

					typedef char byte;

					class Binary
					{
						byte* mBytes;
						std::size_t mLength;
						std::size_t mPosition;

					public:
						Binary(byte* bytes, std::size_t length)
							: mBytes(bytes)
							, mLength(length)
							, mPosition(0)
						{
						}

						template<typename T>
						void Read(T& t)
						{
							read(reinterpret_cast<byte*>(&t), sizeof(T));
						}

						bool Continue()
						{
							return mPosition < mLength;
						}

						void read(byte* buffer, std::size_t size)
						{
							byte* b = getBytes(size);
							memcpy(buffer, b, size);
						}

						byte* getBytes(std::size_t size)
						{
							const std::size_t newPosition = size + mPosition;
							if( newPosition > mLength ) throw "not enough data in buffer...";
							byte* result = mBytes+mPosition;
							mPosition = newPosition;
							return result;
						}


						string ReadString()
						{
							ostringstream s;
							tchar c;
							do 
							{
								read(&c, 1);
								if( c!= 0) s << c;
							} while(c != 0 );
							return s.str();
						}

						int int1()
						{
							uint8 i;
							Read<>(i);
							return i;
						}

						int int2()
						{
							uint16 i;
							Read<>(i);
							return i;
						}
						int int4()
						{
							uint32 i;
							Read<>(i);
							return i;
						}

						float rfloat()
						{
							float i;
							Read<>(i);
							return i;
						}
					};

					struct BinaryChunk;
					typedef boost::shared_ptr<BinaryChunk> BinaryChunkPtr;

					struct BinaryChunk
					{
						BinaryChunk(int aid, byte* abytes, std::size_t length)
							: id(aid)
							, bytes(abytes)
							, mLength(length)
						{
						}

						int id;
						byte* bytes;
						std::size_t mLength;

						Binary getBinary()
						{
							return Binary(bytes, mLength);
						}

						static std::vector<BinaryChunkPtr> Parse(byte* bytes, const std::size_t size)
						{
							std::vector<BinaryChunkPtr> res;
							Binary b(bytes, size);
							ReadFromBinary(res, b);
							return res;
						}

						std::vector<BinaryChunkPtr> SubChunks()
						{
							return Parse(bytes, mLength);
						}

						BinaryChunkPtr getSub(int id)
						{
							std::vector<BinaryChunkPtr> s = SubChunks();
							return SelectChunk(id, s);
						}

						static BinaryChunkPtr SelectChunk(int id, std::vector<BinaryChunkPtr>& s)
						{
							BinaryChunkPtr c = SelectChunkOrNull(id, s);
							if (c.get() == 0) throw "Missing required chunk";
							return c;
						}

						static std::vector<BinaryChunkPtr> IterateChunks(int id, std::vector<BinaryChunkPtr> s)
						{
							std::vector<BinaryChunkPtr> res;
							BOOST_FOREACH(BinaryChunkPtr& c, s)
							{
								if (c->id == id)
								{
									res.push_back(c);
								}
							}
							return res;
						}

						static BinaryChunkPtr SelectChunkOrNull(int id, std::vector<BinaryChunkPtr>& s)
						{
							BOOST_FOREACH(BinaryChunkPtr& c, s)
							{
								if (c->id == id) return c;
							}
							return BinaryChunkPtr();
						}

						static void ReadFromBinary(std::vector<BinaryChunkPtr>& chunks, Binary& b)
						{
							while (b.Continue())
							{
								int id = b.int2();
								int count = b.int4();
								const std::size_t size = count - (2 + 4);
								chunks.push_back(BinaryChunkPtr(new BinaryChunk(id, b.getBytes(size), size)));
							}
						}
					};




					namespace ChunkId
					{
						const int MAIN_CHUNK = 0x4D4D;
						const int EDITOR_3D_CHUNK = 0x3D3D;
						const int OBJECT_BLOCK = 0x4000;
						const int TRIANGULAR_MESH = 0x4100;
						const int VERTICES_LIST = 0x4110;
						const int FACES_DESCRIPTION = 0x4120;
						const int FACES_MATERIAL = 0x4130;
						const int MAPPING_COORDINATES_LIST = 0x4140;
						const int SMOOTHING_GROUP_LIST = 0x4150;
						const int LOCAL_COORDINATES_SYSTEM = 0x4160;
						const int LIGHT = 0x4600;
						const int SPOTLIGHT = 0x4610;
						const int CAMERA = 0x4700;
						const int MATERIAL_BLOCK = 0xAFFF;
						const int MATERIAL_NAME = 0xA000;
						const int AMBIENT_COLOR = 0xA010;
						const int DIFFUSE_COLOR = 0xA020;
						const int SPECULAR_COLOR = 0xA030;
						const int TEXTURE_MAP_1 = 0xA200;
						const int BUMP_MAP = 0xA230;
						const int REFLECTION_MAP = 0xA220;
						// const int [SUB CHUNKS FOR EACH MAP]
						const int MAPPING_FILENAME = 0xA300;
						const int MAPPING_PARAMETERS = 0xA351;
						const int KEYFRAMER_CHUNK = 0xB000;
						const int MESH_INFORMATION_BLOCK = 0xB002;
						const int SPOT_LIGHT_INFORMATION_BLOCK = 0xB007;
						const int FRAMES /*(START AND END)*/ = 0xB008;
						const int OBJECT_NAME = 0xB010;
						const int OBJECT_PIVOT_POINT = 0xB013;
						const int POSITION_TRACK = 0xB020;
						const int ROTATION_TRACK = 0xB021;
						const int SCALE_TRACK = 0xB022;
						const int HIERARCHY_POSITION = 0xB030;

						const int COLOR_RGB = 0x0010;
						const int COLOR_TRU = 0x0011;
						const int COLOR_TRUG = 0x0012;

						const string ToString(int id)
						{
							switch(id)
							{
							case MAIN_CHUNK : return "MAIN_CHUNK";
							case EDITOR_3D_CHUNK : return "EDITOR_3D_CHUNK";
							case OBJECT_BLOCK : return "OBJECT_BLOCK";
							case TRIANGULAR_MESH : return "TRIANGULAR_MESH";
							case VERTICES_LIST : return "VERTICES_LIST";
							case FACES_DESCRIPTION : return "FACES_DESCRIPTION";
							case FACES_MATERIAL : return "FACES_MATERIAL";
							case MAPPING_COORDINATES_LIST : return "MAPPING_COORDINATES_LIST";
							case SMOOTHING_GROUP_LIST : return "SMOOTHING_GROUP_LIST";
							case LOCAL_COORDINATES_SYSTEM : return "LOCAL_COORDINATES_SYSTEM";
							case LIGHT : return "LIGHT";
							case SPOTLIGHT : return "SPOTLIGHT";
							case CAMERA : return "CAMERA";
							case MATERIAL_BLOCK : return "MATERIAL_BLOCK";
							case MATERIAL_NAME : return "MATERIAL_NAME";
							case AMBIENT_COLOR : return "AMBIENT_COLOR";
							case DIFFUSE_COLOR : return "DIFFUSE_COLOR";
							case SPECULAR_COLOR : return "SPECULAR_COLOR";
							case TEXTURE_MAP_1 : return "TEXTURE_MAP_1";
							case BUMP_MAP : return "BUMP_MAP";
							case REFLECTION_MAP : return "REFLECTION_MAP";
							case MAPPING_FILENAME : return "MAPPING_FILENAME";
							case MAPPING_PARAMETERS : return "MAPPING_PARAMETERS";
							case KEYFRAMER_CHUNK : return "KEYFRAMER_CHUNK";
							case MESH_INFORMATION_BLOCK : return "MESH_INFORMATION_BLOCK";
							case SPOT_LIGHT_INFORMATION_BLOCK : return "SPOT_LIGHT_INFORMATION_BLOCK";
							case FRAMES: return "FRAMES (START AND END)";
							case OBJECT_NAME : return "OBJECT_NAME";
							case OBJECT_PIVOT_POINT : return "OBJECT_PIVOT_POINT";
							case POSITION_TRACK : return "POSITION_TRACK";
							case ROTATION_TRACK : return "ROTATION_TRACK";
							case SCALE_TRACK : return "SCALE_TRACK";
							case HIERARCHY_POSITION : return "HIERARCHY_POSITION";
							default: return "???";
							}
						}
					};


					real& RefMat(real* m, int i, int j)
					{
						return m[i+4*j];
					}

					struct LocalCoordinateSystemChunk
					{
						LocalCoordinateSystemChunk()
							: mat( mat44Identity() )
						{
						}

						mat44 mat;

						static mat44 GetMatrix(BinaryChunkPtr c)
						{
							if( c.get() )
							{
								if (c->id != ChunkId::LOCAL_COORDINATES_SYSTEM) throw "not a coordsys";

								Binary b = c->getBinary();
								real lmat[4*4];

								for (int j = 0; j < 4; j++)
								{
									for (int i = 0; i < 3; i++)
									{
										RefMat(lmat, i, j) = b.rfloat();
									}
								}
								RefMat(lmat, 3, 0) = 0;
								RefMat(lmat, 3, 1) = 0;
								RefMat(lmat, 3, 2) = 0;
								RefMat(lmat, 3, 3) = 1;

								return mat44_FromRowMajor(lmat);
							}
							else
							{
								return mat44Identity();
							}
						}

						void load(BinaryChunkPtr c)
						{
							mat = GetMatrix(c);
						}

						vec3 translate(vec3 p) const
						{
							return mat * p;
						}
					};

					struct FaceMaterialChunk
					{
						string name;
						std::vector<int> faces;
						FaceMaterialChunk& load(BinaryChunkPtr c)
						{
							if (c->id != ChunkId::FACES_MATERIAL) throw "not a face material chunk";
							Binary b = c->getBinary();
							name = b.ReadString();
							int count = b.int2();
							for (int i = 0; i < count; ++i)
							{
								faces.push_back(b.int2());
							}

							return *this;
						}
					};


					struct TriMeshChunk
					{
						struct Poly
						{
							Poly(int aa, int bb, int cc)
								: a(aa)
								, b(bb)
								, c(cc)
							{
							}

							pwn::mesh::Triangle::index a;
							pwn::mesh::Triangle::index b;
							pwn::mesh::Triangle::index c;
						};

						std::vector<vec3> points;
						std::vector<Poly> faces;
						std::vector<vec2> mapping;
						std::vector<FaceMaterialChunk> facematerials;
						LocalCoordinateSystemChunk coordsys;

						void load(BinaryChunkPtr c)
						{
							if (c->id != ChunkId::TRIANGULAR_MESH) throw "Not a trimesh";
							std::vector<BinaryChunkPtr> chunks = c->SubChunks();
							points = ParsePoints( BinaryChunk::SelectChunk(ChunkId::VERTICES_LIST, chunks) );
							mapping = ParseMapping(BinaryChunk::SelectChunk(ChunkId::MAPPING_COORDINATES_LIST, chunks));
							std::vector<BinaryChunkPtr> facechunks;
							faces = ParseFaces(BinaryChunk::SelectChunk(ChunkId::FACES_DESCRIPTION, chunks), facechunks);
							BOOST_FOREACH(const BinaryChunkPtr& bc, BinaryChunk::IterateChunks(ChunkId::FACES_MATERIAL, facechunks))
							{
								facematerials.push_back( FaceMaterialChunk().load(bc) );
							}
							coordsys.load(BinaryChunk::SelectChunkOrNull(ChunkId::LOCAL_COORDINATES_SYSTEM, chunks));
						}

						std::vector<Poly> ParseFaces(BinaryChunkPtr c, std::vector<BinaryChunkPtr>& chunks)
						{
							Binary b = c->getBinary();
							int facecount = b.int2();

							std::vector<Poly> res;
							for (int i = 0; i < facecount; ++i)
							{
								int x = b.int2();
								int y = b.int2();
								int z = b.int2();
								b.int2(); // flags
								res.push_back(Poly(x, y, z));
							}
							BinaryChunk::ReadFromBinary(chunks, b);

							return res;
						}

						std::vector<vec2> ParseMapping(BinaryChunkPtr c)
						{
							Binary b = c->getBinary();
							int mapcount = b.int2();

							std::vector<vec2> res;
							for (int i = 0; i < mapcount; ++i)
							{
								float x = b.rfloat();
								float y = b.rfloat();
								res.push_back(vec2(x, y));
							}

							return res;
						}

						std::vector<vec3> ParsePoints(BinaryChunkPtr c)
						{
							Binary b = c->getBinary();
							int points = b.int2();

							std::vector<vec3> res;

							for (int i = 0; i < points; ++i)
							{
								float x = b.rfloat();
								float z = -b.rfloat();
								float y = b.rfloat();
								res.push_back(vec3(x, y, z));
							}

							return res;
						}
					};


					struct StringChunk
					{
						string Value;
						void load(BinaryChunkPtr c)
						{
							Value = c->getBinary().ReadString();
						}
					};

					struct ColorChunk
					{
						float red;
						float green;
						float blue;

						void load(BinaryChunkPtr c)
						{
							if (c.get() != 0)
							{
								Binary b = c->getBinary();
								const int type = b.int2();
								const int size = b.int4();
								switch(type)
								{
								case ChunkId::COLOR_RGB:
								case ChunkId::COLOR_TRU:
								case ChunkId::COLOR_TRUG:
									red = b.int1() / 255.0f;
									green = b.int1() / 255.0f;
									blue = b.int1() / 255.0f;
									break;
								default:
									throw "Unknown color...";
								}
							}
							else
							{
								red = 1;
								green = 1;
								blue = 1;
							}
						}

						vec3 Color()
						{
							return vec3(red, green, blue);
						}
					};


					struct ObjectChunk
					{
						TriMeshChunk trimesh;
						string name;

						ObjectChunk& load(BinaryChunkPtr c)
						{
							if (c->id != ChunkId::OBJECT_BLOCK) throw "Not a object block";

							std::vector<BinaryChunkPtr> chunks;
							Binary b = c->getBinary();
							name = b.ReadString();
							BinaryChunk::ReadFromBinary(chunks, b);
							BinaryChunkPtr tm = BinaryChunk::SelectChunk(ChunkId::TRIANGULAR_MESH, chunks);
							if (tm.get() == 0) throw "Missing a trimesh chunk";
							trimesh.load(tm);

							return *this;
						}
					};

					struct MaterialChunk
					{
						StringChunk name;
						ColorChunk ambient;
						ColorChunk diffuse;
						ColorChunk specular;
						StringChunk texture;


						MaterialChunk& load(BinaryChunkPtr c)
						{
							if (c->id != ChunkId::MATERIAL_BLOCK) throw "Not a material chunk";
							std::vector<BinaryChunkPtr> chunks = c->SubChunks();

							name.load(BinaryChunk::SelectChunk(ChunkId::MATERIAL_NAME, chunks) );
							ambient.load(BinaryChunk::SelectChunkOrNull(ChunkId::AMBIENT_COLOR, chunks) );
							diffuse.load(BinaryChunk::SelectChunkOrNull(ChunkId::DIFFUSE_COLOR, chunks) );
							specular.load(BinaryChunk::SelectChunkOrNull(ChunkId::SPECULAR_COLOR, chunks) );
							texture.load(BinaryChunk::SelectChunk(ChunkId::TEXTURE_MAP_1, chunks)->getSub(ChunkId::MAPPING_FILENAME));

							return *this;
						}
					};




					struct Editor3dChunk
					{
						std::vector<ObjectChunk> objects;
						std::vector<MaterialChunk> materials;

						void load(BinaryChunkPtr c)
						{
							if (c->id != ChunkId::EDITOR_3D_CHUNK) throw "Not a 3d editor chunk";
							std::vector<BinaryChunkPtr> chunks = c->SubChunks();

							BOOST_FOREACH(const BinaryChunkPtr& oc , BinaryChunk::IterateChunks(ChunkId::OBJECT_BLOCK, chunks))
							{
								objects.push_back( ObjectChunk().load(oc) );
							}
							BOOST_FOREACH (const BinaryChunkPtr& mc , BinaryChunk::IterateChunks(ChunkId::MATERIAL_BLOCK, chunks))
							{
								materials.push_back( MaterialChunk().load(mc));
							}
						}
					};


					struct MainChunk
					{
						Editor3dChunk editor;

						void load(BinaryChunkPtr c)
						{
							if (c->id != ChunkId::MAIN_CHUNK) throw "Not a main chunk";
							BinaryChunkPtr ed = c->getSub(ChunkId::EDITOR_3D_CHUNK);
							if (ed.get() == 0) throw "Missing 3d editor chunk";
							editor.load(ed);
						}
					};




					static void ParseChunk(OptimizedMeshBuilder* builder, MainChunk& main)
					{
						BOOST_FOREACH(MaterialChunk& chunk, main.editor.materials)
						{
							pwn::mesh::Mesh::MaterialPtr mat( new pwn::mesh::Material() );
							builder->mBuilder.addMaterial(mat);
							mat->name = chunk.name.Value;
							mat->texture_diffuse = chunk.texture.Value;
							//mat.specular = chunk.specular.Color;
							//mat.ambient = chunk.ambient.Color;
							//mat.diffuse = chunk.diffuse.Color;
						}

						BOOST_FOREACH(ObjectChunk chunk , main.editor.objects)
						{
							pwn::mesh::Triangle::index ibase = builder->mesh()->positions.size();
							for (std::size_t i = 0; i < chunk.trimesh.points.size(); ++i)
							{
								builder->addPosition( pwn::mesh::Point(chunk.trimesh.coordsys.translate(chunk.trimesh.points[i]), 0) );
								builder->addTextCoord(chunk.trimesh.mapping[i]);
							}
							BOOST_FOREACH(FaceMaterialChunk fmc , chunk.trimesh.facematerials)
							{
								pwn::uint32 material = builder->getMaterial(fmc.name);
								BOOST_FOREACH(int faceindex, fmc.faces)
								{
									const TriMeshChunk::Poly p = chunk.trimesh.faces[faceindex];

									const pwn::mesh::Triangle::index a = ibase + p.a;
									const pwn::mesh::Triangle::index b = ibase + p.b;
									const pwn::mesh::Triangle::index c = ibase + p.c;

									builder->mBuilder.addTriangle(material, pwn::mesh::Triangle( 
												  pwn::mesh::Triangle::Vertex(a, 0, a)
												, pwn::mesh::Triangle::Vertex(b, 0, b)
												, pwn::mesh::Triangle::Vertex(c, 0, c)
											)
										);
								}
							}
						}
					}

					void Load(OptimizedMeshBuilder* builder, const string& filename)
					{
						std::vector<BinaryChunkPtr> chunks;

						std::ifstream f;

						f.open(filename.c_str(), std::ios::binary);
						f.seekg(0, std::ios::end);
						const std::streamsize length = f.tellg();
						boost::scoped_array<byte> bytes( new byte[length] );
						f.seekg(0, std::ios::beg);
						f.read(bytes.get(), length);

						Binary b(bytes.get(), length);
						BinaryChunk::ReadFromBinary(chunks, b);

						if (chunks.size() != 1) throw "Unknown or damaged 3ds file";

						MainChunk main;
						main.load(chunks[0]);
						ParseChunk(builder, main);
						builder->done();
					}

				}
			}

			void read(OptimizedMeshBuilder* builder, const pwn::string& file)
			{
				hidden::Load(builder, file);
				builder->mBuilder.buildNormals();
				//pwn::mesh::BuildNormals(builder->mesh());
			}
		}
	}
}