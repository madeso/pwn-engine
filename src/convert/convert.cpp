#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include <pwn/mesh/mesh.h>
#include <pwn/io/io.h>
#include <pwn/mesh/builder.h>
#include <boost/foreach.hpp>

#include "convert.hpp"
#include "inputformat.hpp"

#include <pwn/assert.h>
#include <pwn/core/vec.h>
#include <pwn/core/stringutils.h>
#include <pwn/core/consolearguments.h>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace pwn
{
	namespace convert
	{
		Entry::Entry(const mesh::Builder& b, const string& n)
			: builder(b)
			, name(n)
		{
		}

		AnimationEntry::AnimationEntry(const mesh::Animation& a, const string& n)
			: animation(a)
			, name(n)
		{
		}

		const InputFormat* SuggestFormat(const pwn::string& inputfile, const InputFormat* formatOveride)
		{
			if( formatOveride != 0 ) return formatOveride;
			const pwn::string ext = boost::filesystem::path(inputfile).extension();

			return SuggestFormat(ext);
		}

		const InputFormat* SuggestFormatData(const pwn::string& name)
		{
			return SuggestFormat("." + name);
		}

		/**
		 Supports: 3ds, obj, milkshape binary & milkshape ascii.
		 Add support for x, collada, md2, md3, md5, an8, ogre mesh, dxf & blender
		*/
		bool Load(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& inputfile, const InputFormat* formatOveride, bool verbose)
		{
			const InputFormat* fileFormat = SuggestFormat(inputfile, formatOveride);

			if(fileFormat)
			{
				if( verbose ) cout << "reading " << inputfile << " (" << fileFormat->getName() << ").." << std::endl;
				fileFormat->load(builders, subobjects, inputfile, verbose);
				return true;
			}
			else
			{
				std::cerr << "Unable to determine the kind of reader to use with " << inputfile;
				return false;
			}
		}

		typedef std::vector<pwn::mesh::AnimationInformation> AnimationExtract;

		pwn::string SuggestTextureDirectory(const pwn::string& in)
		{
			return boost::filesystem::path(in).replace_extension().filename();
		}

		bool SuggestAnimationFile(const pwn::string& in, pwn::string* out)
		{
			boost::filesystem::path p
				= boost::filesystem::path(in).replace_extension("animinfo");
			if( out ) *out = p.file_string();
			return boost::filesystem::exists(p);
		}

		pwn::string GetAbsolutePath(const pwn::string& in)
		{
			return boost::filesystem::system_complete( boost::filesystem::path( in.c_str(), boost::filesystem::native ) ).string();
		}

		pwn::string SuggestOutDirectory(const pwn::string& in)
		{
			return boost::filesystem::path(in).remove_filename().directory_string();
		}

		mesh::Animation ExtractDefaultAnimation(const mesh::Builder& builder)
		{
			std::vector<mesh::AnimationPerBone> apb;

			BOOST_FOREACH(const mesh::Bone& bone, builder.bones)
			{
				mesh::AnimationPerBone a;
				a.addRotation(0, bone.rot);
				a.addRotation(1, bone.rot);
				a.addPosition(0, bone.pos);
				a.addPosition(1, bone.pos);
				apb.push_back(a);
			}

			return mesh::Animation(apb);
		}

		struct ConvertData
		{
			ConvertData()
				: useModelScale(false)
				, modelScale(1)
				, verbose(false)
				, meshInfo(false)
				, writeResult(true)
				, writeDefaultAnimation(true)
			{
			}

			bool useModelScale;
			float modelScale;
			pwn::string texturedir; // relative directory when loading textures from game
			pwn::string animdir; // if empty a default based on outdir is created
			pwn::string outdir; // if emppy a default based on input file is created
			bool runStatistics;
			bool verbose;
			bool meshInfo;
			bool writeResult;
			bool writeDefaultAnimation;
		};

		pwn::string CombineFilename(const pwn::string& name, const pwn::string& object)
		{
			if( object.empty() ) return name;
			else if( object == name ) return name;
			else return name + "-" + object;
		}

		AnimationExtract LoadAnimations(const pwn::string& filename)
		{
			using boost::property_tree::ptree;

			AnimationExtract ae;
			ptree pt;
			read_info(filename, pt);
			BOOST_FOREACH(ptree::value_type &an, pt.get_child("animations"))
			{
				ptree anida = an.second;
				const pwn::string name = anida.get<pwn::string>("name");
				const pwn::uint32 start = anida.get<pwn::uint32>("start");
				const pwn::uint32 end = anida.get<pwn::uint32>("end");
				ae.push_back(pwn::mesh::AnimationInformation(start, end, name));
			}

			return ae;
		}

		bool Convert(const pwn::string& argv0, const ConvertData& cd, const std::vector<pwn::string>& subobjects, const pwn::string& ainputfile, const InputFormat* formatOveride)
		{
			const pwn::string inputfile = GetAbsolutePath(ainputfile);
			const pwn::string outdir =
				cd.outdir.empty() ? SuggestOutDirectory(inputfile) : cd.outdir;
			BuilderList builders;
			if( Load(&builders, subobjects, inputfile, formatOveride, cd.verbose) == false ) return false;

			BOOST_FOREACH(Entry& e, builders)
			{
				mesh::Builder& builder = e.builder;
				//if( builder.normals.empty() ) 
				builder.buildNormals();
				const string name = e.name;
				pwn::mesh::Flatouter flatouter(builder);
				flatouter.modify(&builder);
				
				BOOST_FOREACH(AnimationEntry& ae, e.animations)
				{
					flatouter.modify(&ae.animation);
				}

				if( cd.useModelScale )
				{
					if( cd.verbose ) cout << "scaling " << cd.modelScale << ".." << std::endl;
					pwn::mesh::Scale(&builder, cd.modelScale);
					BOOST_FOREACH(AnimationEntry& ae, e.animations)
					{
						ae.animation.scale(cd.modelScale);
					}
				}

				if( cd.verbose ) cout << endl;

				pwn::string texturedir = cd.texturedir == "@" ? SuggestTextureDirectory(inputfile) : cd.texturedir;
				pwn::mesh::MoveTextures(&builder, texturedir);

				if( cd.writeDefaultAnimation && e.builder.bones.empty() == false)
				{
					e.animations.push_back(AnimationEntry(ExtractDefaultAnimation(e.builder), "_default"));
				}

				pwn::mesh::Mesh mesh = builder.asMesh();
				const pwn::uint32 validationErrors = mesh.validate(true);
				if( validationErrors != 0)
				{
					cerr << inputfile << " failed validation with " << validationErrors << " error(s)... ignoring file..." << endl;
					return false;
				}

				if( cd.meshInfo )
				{
					cout
						<< "Mesh information " << name << ":" << endl
						<< " positions: " << mesh.data().getCount() << endl
						<< " materials: " << mesh.getMaterials().size() << endl
						<< " triangles: " << mesh.getNumberOfTriangles() << endl
						<< endl;
				}

				if( cd.writeResult )
				{
					if( cd.verbose ) cout << "writing.." << endl;

					pwn::string fname = CombineFilename(boost::filesystem::path(inputfile).stem(), name);

					{
						pwn::io::WriteTarget wt(argv0, outdir);
						pwn::io::Write(mesh, fname + ".mesh");
					}

					AnimationList anis;

					AnimationExtract animationsToExtract;
					pwn::string animationfile;
					if( SuggestAnimationFile(inputfile, &animationfile) )
					{
						animationsToExtract = LoadAnimations(animationfile);

						BOOST_FOREACH(AnimationEntry& ae, e.animations)
						{
							if( ae.name.empty() == false )
							{
								anis.push_back(ae);
							}
							else
							{
								BOOST_FOREACH(const pwn::mesh::AnimationInformation& ai, animationsToExtract)
								{
									pwn::mesh::Animation ani;
									ae.animation.subanim(ai, &ani);
									anis.push_back( AnimationEntry(ani, ai.name));
								}
							}
						}
					}
					else
					{
						anis = e.animations;
					}

					BOOST_FOREACH(AnimationEntry& ae, anis)
					{
						pwn::string adir = cd.animdir;
						if( adir.empty() )
						{
							adir = (
								boost::filesystem::path(outdir)
								/ fname
								)
								.directory_string();
						};

						{
							pwn::io::WriteTarget wt(argv0, adir);
							pwn::io::Write(ae.animation, boost::filesystem::path(ae.name).replace_extension("anim").filename());
						}
					}
				}
			}

			if( cd.verbose ) cout << endl << "done." << endl;

			return true;
		}

		class App
		{
		public:
			ConvertData arg;
			ConvertData old;
			const InputFormat* formatOveride;
			int errors;
			std::vector<pwn::string> subobjects;

			App()
				: formatOveride(0)
				, errors(0)
			{
			}

			void handle(int argc, char* argv[]);
		};

		int CommandArg_Scale(App* app, core::ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.modelScale = boost::lexical_cast<float>(val);
			app->arg.useModelScale = true;
			return 0;
		}

		int CommandArg_TextureDir(App* app, core::ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.texturedir = val;
			return 0;
		}

		int CommandArg_AnimDir(App* app, core::ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.animdir = val;
			return 0;
		}

		int CommandArg_DisplayHelp(App* app, core::ConsoleArguments<App>* args, const pwn::string& val)
		{
			const pwn::string usage = args->displayUsage(val);

			if( usage.empty() )
			{
				std::cerr << "Unknown command " << val << std::endl;
				args->stop = true;
				return 1;
			}

			std::cout << "Usage: " << usage << std::endl
				<< "Aliases: " << args->displayAliases(val) << std::endl
				<< "Description: " << args->displayDescription(val) << std::endl;

			args->stop = true;
			return 0;
		}

		int CommandArg_OutDir(App* app, core::ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.outdir = val;
			return 0;
		}

		int CommandArg_WriteDefaultAnim(App* app, core::ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.writeDefaultAnimation = val=="Y" || val =="y";
			return 0;
		}

		int Command_RunStatistics(App* app, core::ConsoleArguments<App>* args)
		{
			app->arg.runStatistics = true;
			return 0;
		}

		int Command_MeshInfo(App* app, core::ConsoleArguments<App>* args)
		{
			app->arg.meshInfo = true;
			return 0;
		}

		int Command_DontWriteResult(App* app, core::ConsoleArguments<App>* args)
		{
			app->arg.writeResult = false;
			return 0;
		}

		int Command_Verbose(App* app, core::ConsoleArguments<App>* args)
		{
			app->arg.verbose = true;
			return 0;
		}
		int Command_Restore(App* app, core::ConsoleArguments<App>* args)
		{
			app->arg = app->old;
			return 0;
		}

		int CommandArg_ForceFormat(App* app, core::ConsoleArguments<App>* args, const pwn::string& val)
		{
			const InputFormat* sf = SuggestFormatData(val);
			if( sf )
			{
				app->formatOveride = sf;
				return 0;
			}
			else
			{
				std::cerr << "Unknown format override: " << val << " using last override";
				return 1;
			}
		}

		int CommandArg_AddSubObject(App* app, core::ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->subobjects.push_back(val);
			return 0;
		}

		int Command_DontStopOnErrors(App* app, core::ConsoleArguments<App>* args)
		{
			args->stopOnError = false;
			return 0;
		}

		int Command_DisplayArguments(App* app, core::ConsoleArguments<App>* args)
		{
			args->displayCommands();
			args->stop = true;
			return 0;
		}

		int CommandArg_RunFile(App* app, core::ConsoleArguments<App>* args, const pwn::string& file)
		{
			try
			{
				Convert(args->argv0, app->arg, app->subobjects, file, app->formatOveride);
			}
			catch(const std::exception& ex)
			{
				std::cerr << "Error: " << ex.what() << std::endl;
				return 1;
			}
			catch(const char* msg)
			{
				std::cerr << "Error: " << msg << std::endl;
				return 1;
			}
			catch(const std::string& msg)
			{
				std::cerr << "Error: " << msg << std::endl;
				return 1;
			}
			catch(...)
			{
				std::cerr << "Unknown error detected!" << std::endl;
				return 1;
			}
			app->old = app->arg;
			app->arg = ConvertData();
			app->formatOveride = 0;
			app->subobjects.clear();

			return 0;
		}

		void App::handle(int argc, char* argv[])
		{
			core::ConsoleArguments<App> args;

			typedef core::Vec<pwn::string> Strings;

			args.setArgCommand(Strings() << "scale" << "s", CommandArg_Scale, "scales the loaded mesh");
			args.setArgCommand(Strings() << "texturedir" << "tex" << "t", CommandArg_TextureDir, "sets the texture dir that will be used when loading, @ means meshfilename");
			args.setArgCommand(Strings() << "animdir" << "anim" << "a", CommandArg_AnimDir, "sets the animation dir");
			args.setArgCommand(Strings() << "outdir" << "out" << "o", CommandArg_OutDir, "sets the outdir where to place the result, defaults to meshdir");
			args.setArgCommand(Strings() << "format" << "force" << "f", CommandArg_ForceFormat, "");
			args.setArgCommand(Strings() << "run-file" << "run" << "F", CommandArg_RunFile, "forces a file to run, needed if the path starts with / or -");
			args.setArgCommand(Strings() << "object" << "obj" << "O", CommandArg_AddSubObject, "adds a subobject to be converted, if not set all will be converted");

			args.setArgCommand(Strings() << "more" << "help-about" << "H", CommandArg_DisplayHelp, "Shows help about a command");

			args.setArgCommand(Strings() << "default-anim" << "def" << "d", CommandArg_WriteDefaultAnim, "If Y or y, then the default animation is written (default y)");

			args.setCommand(Strings() << "showstat" << "stat" << "S", Command_RunStatistics, "Show statistics for the mesh");
			args.setCommand(Strings() << "info" << "i", Command_MeshInfo, "Show mesh informatio");
			args.setCommand(Strings() << "nowrite" << "w", Command_DontWriteResult, "Only run logic, dont write the result");
			args.setCommand(Strings() << "verbose" << "v", Command_Verbose, "Switch to verbose mode");
			args.setCommand(Strings() << "keep" << "k", Command_Restore, "Keep the options for the last mesh");
			args.setCommand(Strings() << "no-break-on-error" << "no-break" << "B", Command_DontStopOnErrors, "Forces convertion to run even after errors");
			args.setCommand(Strings() << "help" << "h", Command_DisplayArguments, "Shows a list of commands");

			args.setMain(CommandArg_RunFile);

			args.handle(argc, argv,this);
			errors = args.getErrors();
		}
	}
}

int main(int argc, char* argv[])
{
	using namespace std;

	if( argc == 1 )
	{
		cout << "error usage: pwn-convert filename arguments";
		return 1;
	}

	pwn::convert::App a;
	a.handle(argc, argv);

	return a.errors;
}

