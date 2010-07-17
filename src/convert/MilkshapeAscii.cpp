#include <pwn/string.h>
#include "MilkshapeCommon.hpp"
#include <vector>
#include <pwn/core/StringUtils.h>
#include <boost/lexical_cast.hpp>
#include <fstream>

namespace pwn
{
	namespace convert
	{
		namespace milkshape
		{
			namespace ascii
			{
				using namespace pwn::convert::milkshape::common;
				using namespace pwn::core;

				int intParse(const string& s)
				{
					const string str = Trim(s);
					return boost::lexical_cast<int>(str);
				}

				real floatParse(const string& s)
				{
					const string str = Trim(s);
					return boost::lexical_cast<real>(str);
				}

				// removes " at start and end
				string Cleanup(string p)
				{
					return p.substr(1, p.size()-2);
				}
				class Runner : boost::noncopyable
				{
				public:
					Model model;
					const std::vector<string>& mLines;
					const real scale;
					std::size_t currentLine;

					Runner(const std::vector<string>& aLines, real aScale)
						: mLines(aLines)
						, scale(aScale)
						, currentLine(0)
					{
					}

					void run()
					{
						while (currentLine < mLines.size())
						{
							string l = readLine();

							if (StartsWith(l, "//")) continue;
							if( Trim(l) == "" ) continue;
							std::vector<string> cmd = SplitString(l, ":");
							if (cmd[0] == "Frames")
							{
								readFrameCount(cmd);
							}
							else if (cmd[0] == "Frame")
							{
								readCurrentFrame(cmd);
							}
							else if (cmd[0] == "Meshes")
							{
								readMeshes(cmd);
							}
							else if (cmd[0] == "Materials")
							{
								readMaterials(cmd);
							}
							else if (cmd[0] == "Bones")
							{
								readBones(cmd);
							}
						}
						model.mapBonesToId();
					}

					void readBones(const std::vector<string>& cmd)
					{
						int bonecount = intParse(cmd[1]);
						for (int boneId = 0; boneId < bonecount; ++boneId)
						{
							Bone& bone = model.newBone();
							bone.name = Cleanup(readLine());
							bone.parentName = Cleanup(readLine());

							std::vector<string> data = SplitString(readLine(), " ");
							bone.flags = intParse(data[0]);
							bone.x = floatParse(data[1]) * scale;
							bone.y = floatParse(data[2]) * scale;
							bone.z = floatParse(data[3]) * scale;
							bone.rx = floatParse(data[4]);
							bone.ry = floatParse(data[5]);
							bone.rz = floatParse(data[6]);

							readPositionFrames(bone);
							readRotationFrame(bone);
						}
					}

					void readRotationFrame(Bone& bone)
					{
						string textcount = readLine();
						int count = intParse(textcount);
						for (int frameId = 0; frameId < count; ++frameId)
						{
							std::vector<string> data = SplitString(readLine(), " ");
							RotationKey key = bone.newRotationKey();
							key.time = floatParse(data[0]);
							key.x = floatParse(data[1]);
							key.y = floatParse(data[2]);
							key.z = floatParse(data[3]);
						}
					}

					void readPositionFrames(Bone& bone)
					{
						string textcount = readLine();
						int count = intParse(textcount);
						for (int frameId = 0; frameId < count; ++frameId)
						{
							std::vector<string> data = SplitString(readLine(), " ");
							PositionKey key = bone.newPositionKey();
							key.time = floatParse(data[0]);
							key.x = floatParse(data[1]) * scale;
							key.y = floatParse(data[2]) * scale;
							key.z = floatParse(data[3]) * scale;
						}
					}

					void readMaterials(const std::vector<string>& cmd)
					{
						int materialcount = intParse(cmd[1]);
						for(int matId = 0; matId < materialcount; ++matId)
						{
							readSingleMaterial();
						}
					}

					void readSingleMaterial()
					{
						Material& mat = model.newMaterial();
						mat.name = Cleanup(readLine());
						mat.ambient.parse(readLine());
						mat.diffuse.parse(readLine());
						mat.specular.parse(readLine());
						mat.emissive.parse(readLine());
						mat.shininess = floatParse(readLine());
						mat.transperency = floatParse(readLine());
						mat.diffuseTexture = Cleanup(readLine());
						mat.alphatexture = Cleanup(readLine());
					}

					void readMeshes(const std::vector<string>& cmd)
					{
						// number of meshes
						int meshcount = intParse(cmd[1]);
						for (int meshId = 0; meshId < meshcount; ++meshId)
						{
							readSingleMesh();
						}
					}

					void readSingleMesh()
					{
						string meshline = readLine();
						std::vector<string> meshdata = SplitString(meshline, " ");
						Mesh& mesh = model.newMesh();
						mesh.name = Cleanup(meshdata[0]);
						mesh.flags = intParse(meshdata[1]);
						mesh.materialId = intParse(meshdata[2]);

						string vertexline = readLine();
						int vertexcount = intParse(vertexline);
						for (int vertex = 0; vertex < vertexcount; ++vertex)
						{
							readSingleVertex(mesh);
						}

						string normalline = readLine();
						int normalcount = intParse(vertexline);
						for (int normal = 0; normal < normalcount; ++normal)
						{
							readSingleNormal(mesh);
						}

						string triline = readLine();
						int tricount = intParse(triline);
						for (int tri = 0; tri < tricount; ++tri)
						{
							readSingleTriangle(mesh);
						}
					}

					void readSingleTriangle(Mesh& mesh)
					{
						string triline = readLine();
						std::vector<string> tricmd = SplitString(triline, " ");
						Tri& tri = mesh.newTri();
						tri.flags = intParse(tricmd[0]);
						tri.v1 = intParse(tricmd[1]);
						tri.v2 = intParse(tricmd[2]);
						tri.v3 = intParse(tricmd[3]);
						tri.n1 = intParse(tricmd[4]);
						tri.n2 = intParse(tricmd[5]);
						tri.n3 = intParse(tricmd[6]);
						tri.smoothingGroup = intParse(tricmd[7]);
						tri.buildNormal(mesh);
					}

					void readSingleNormal(Mesh& mesh)
					{
						string normalline = readLine();
						std::vector<string> normalcmd = SplitString(normalline, " ");
						Normal& n = mesh.newNormal();
						n.norm.x = floatParse(normalcmd[0]);
						n.norm.y = floatParse(normalcmd[1]);
						n.norm.z = floatParse(normalcmd[2]);
						n.normalize();
					}

					void readSingleVertex(Mesh& mesh)
					{
						string vertexline = readLine();
						std::vector<string> vertexcmd = SplitString(vertexline, " ");
						Vertex& v = mesh.newVertex();
						v.flags = intParse(vertexcmd[0]);
						v.pos.x = floatParse(vertexcmd[1]) * scale;
						v.pos.y = floatParse(vertexcmd[2]) * scale;
						v.pos.z = floatParse(vertexcmd[3]) * scale;
						v.uv.x = floatParse(vertexcmd[4]);
						v.uv.y = floatParse(vertexcmd[5]);
						v.bone = intParse(vertexcmd[6]);
					}

					const string readLine()
					{
						if( mLines.size() <= currentLine ) throw "file is to short...";
						string line = mLines[currentLine];
						++currentLine;
						return line;
					}

					void readCurrentFrame(std::vector<string> cmd)
					{
						model.currentFrame = intParse(cmd[1]);
					}

					void readFrameCount(std::vector<string> cmd)
					{
						model.framecount = intParse(cmd[1]);
					}
				};

				void Read(OptimizedMeshBuilder* builder, const pwn::string& path)
				{
					std::ifstream f(path.c_str());
					if( !f ) throw "failed to open file";
					std::vector<string> lines;
					string line;
					while(std::getline(f, line)) lines.push_back(line);
					Runner runner(lines, 1.0f);
					runner.run();
					MilkshapeCommon::ExtractMeshDefinition(runner.model, builder);
					/*def = MilkshapeCommon.ExtractMeshDefinition(runner.model);
					animation = MilkshapeCommon.ExtractAnimation(runner.model);*/
				}
			}
		}
	}
}