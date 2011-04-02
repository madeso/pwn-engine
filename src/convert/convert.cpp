#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include <pwn/mesh/mesh.h>
#include <pwn/io/io.h>
#include <pwn/mesh/builder.h>
#include <boost/foreach.hpp>

#include "WavefrontObj.hpp"
#include "3ds.hpp"
#include "An8.hpp"
#include "convert.hpp"

#include "MilkshapeAscii.hpp"
#include "MilkshapeBinary.hpp"
#include <pwn/assert.h>
#include <pwn/core/stringutils.h>
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>

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

		/** Writes a dot to std::out to display progress..
		*/
		struct WriteDotCallback : public pwn::convert::obj::VoidVoidCallback
		{
			bool verbose;
			explicit WriteDotCallback(bool aVerbose)
				: verbose(aVerbose)
			{
			}

			void perform()
			{
				if( verbose ) std::cout << ".";
			}
		};

		class InputFormat
		{
		public:
			virtual ~InputFormat() {}
			virtual const std::string getName() const = 0;
			virtual void load(BuilderList* builders, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const = 0;
		};

		template<class IF>
		static InputFormat* GetInputFormat()
		{
			static IF format;
			return &format;
		};

		class InputFormat_Obj
			: public InputFormat
		{
		public:
			~InputFormat_Obj()
			{
			}
			const std::string getName() const
			{
				return "wavefront obj";
			}
			void load(BuilderList* builders, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
			{
				WriteDotCallback wdc(verbose);
				pwn::convert::obj::read(builders, inputfile, wdc);
			}
		};

		class InputFormat_Studio3ds
			: public InputFormat
		{
		public:
			~InputFormat_Studio3ds()
			{
			}
			const std::string getName() const
			{
				return "3d studio";
			}
			void load(BuilderList* builders, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
			{
				pwn::convert::studio3ds::read(builders, inputfile);
			}
		};

		class InputFormat_Ms3d_ascii
			: public InputFormat
		{
		public:
			~InputFormat_Ms3d_ascii()
			{
			}
			const std::string getName() const
			{
				return "milkshape ascii";
			}
			void load(BuilderList* builders, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
			{
				pwn::convert::milkshape::ascii::Read(builders, animation, inputfile);
			}
		};

		class InputFormat_An8
			: public InputFormat
		{
		public:
			~InputFormat_An8()
			{
			}
			const std::string getName() const
			{
				return "anim8or";
			}
			void load(BuilderList* builders, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
			{
				pwn::convert::an8::read(builders, inputfile);
			}
		};

		class InputFormat_Ms3d_binary
			: public InputFormat
		{
		public:
			~InputFormat_Ms3d_binary()
			{
			}
			const std::string getName() const
			{
				return "milkshape binary";
			}
			void load(BuilderList* builders, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
			{
				pwn::convert::milkshape::binary::Read(builders, animation, inputfile);
			}
		};

		const InputFormat* SuggestFormat(const pwn::string& ext)
		{
			if( ext == ".obj" ) return GetInputFormat<InputFormat_Obj>();
			else if( ext == ".3ds" ) return GetInputFormat<InputFormat_Studio3ds>();
			else if( ext == ".txt" ) return GetInputFormat<InputFormat_Ms3d_ascii>();
			else if( ext == ".ms3d" ) return GetInputFormat<InputFormat_Ms3d_binary>();
			else if( ext == ".an8" ) return GetInputFormat<InputFormat_An8>();
			else return 0;
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
		bool Load(BuilderList* builders, pwn::mesh::Animation* animation, const pwn::string& inputfile, const InputFormat* formatOveride, bool verbose)
		{
			const InputFormat* fileFormat = SuggestFormat(inputfile, formatOveride);

			if(fileFormat)
			{
				if( verbose ) cout << "reading " << inputfile << " (" << fileFormat->getName() << ").." << std::endl;
				fileFormat->load(builders, animation, inputfile, verbose);
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

		struct ConvertData
		{
			ConvertData()
				: useModelScale(false)
				, modelScale(1)
				, verbose(false)
				, meshInfo(false)
				, writeResult(true)
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
		};

		bool Convert(const pwn::string& argv0, const ConvertData& cd, const AnimationExtract& animationsToExtract, const pwn::string& ainputfile, const InputFormat* formatOveride)
		{
			const pwn::string inputfile = GetAbsolutePath(ainputfile);
			const pwn::string outdir =
				cd.outdir.empty() ? SuggestOutDirectory(inputfile) : cd.outdir;
			BuilderList builders;
			pwn::mesh::Animation animation;
			if( Load(&builders, &animation, inputfile, formatOveride, cd.verbose) == false ) return false;

			BOOST_FOREACH(Entry& e, builders)
			{
				mesh::Builder& builder = e.builder;
				//if( builder.normals.empty() ) 
				builder.buildNormals();
				const string name = e.name;
				pwn::mesh::Flatouter flatouter(builder);
				flatouter.modify(&builder);
				flatouter.modify(&animation);

				if( cd.useModelScale )
				{
					if( cd.verbose ) cout << "scaling " << cd.modelScale << ".." << std::endl;
					pwn::mesh::Scale(&builder, cd.modelScale);
					animation.scale(cd.modelScale);
				}

				if( cd.verbose ) cout << endl;

				pwn::string texturedir = cd.texturedir == "@" ? SuggestTextureDirectory(inputfile) : cd.texturedir;
				pwn::mesh::MoveTextures(&builder, texturedir);

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
					pwn::io::WriteTarget wt(argv0, outdir);
					pwn::io::Write(mesh, boost::filesystem::path(inputfile).replace_extension("mesh").filename());
				}

				BOOST_FOREACH(const pwn::mesh::AnimationInformation& ai, animationsToExtract)
				{
					pwn::mesh::Animation ani;
					animation.subanim(ai, &ani);

					pwn::string adir = cd.animdir;
					if( adir.empty() )
					{
						adir = (
							boost::filesystem::path(outdir)
							/ boost::filesystem::path(inputfile).replace_extension().filename()
							)
							.directory_string();
					};

					pwn::io::WriteTarget wt(argv0, adir);
					pwn::io::Write(ani, boost::filesystem::path(ai.name).replace_extension("anim").filename());
				}
			}

			if( cd.verbose ) cout << endl << "done." << endl;

			return true;
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

		bool IsArgument(const std::string& str)
		{
			Assert(!str.empty());
			return str[0] == '-' || str[0] == '/';
		}

		template <class Main>
		class ConsoleArguments
		{
		public:
			typedef boost::function<int (Main* main, ConsoleArguments* args)> Command;
			typedef std::map<pwn::string, Command> CommandMap;
			typedef boost::function<int (Main* main, ConsoleArguments* args, const pwn::string&)> CommandArg;
			typedef std::map<pwn::string, CommandArg> CommandArgMap;

			typedef std::map<pwn::string, std::vector<pwn::string> > AliasMap;
			typedef std::map<pwn::string, pwn::string> DescriptionMap;

			bool stopOnError;
			bool stop;
			pwn::string argv0;

			ConsoleArguments()
				: errors(0)
				, stopOnError(false)
				, stop(false)
			{
			}
			
			void setCommand(const std::vector<pwn::string>& names, Command arg, const pwn::string& description)
			{
				const pwn::string first = names[0];
				commands.push_back(first);

				BOOST_FOREACH(pwn::string name, names)
				{
					Assert( has(name) == false );
					cmd[name] = arg;
					descriptions[name] = description;
					aliases[name] = names;
				}
			}

			void setArgCommand(const std::vector<pwn::string>& names, CommandArg arg, const pwn::string& description)
			{
				const pwn::string first = names[0];
				commands.push_back(first);

				BOOST_FOREACH(pwn::string name, names)
				{
					Assert( has(name) == false );
					cmda[name] = arg;
					descriptions[name] = description;
					aliases[name] = names;
				}
			}

			void setMain(CommandArg arg)
			{
				cmdmain = arg;
			}

			bool has(const pwn::string& name) const
			{
				const bool missing = cmd.find(name) == cmd.end() && cmda.find(name)==cmda.end();
				return !missing;
			}

			void displayCommands()
			{
				BOOST_FOREACH(const pwn::string& n, commands)
				{
					cout << n << " ";
				}
				cout << std::endl;
			}

			const pwn::string displayAliases(const pwn::string& id) const
			{
				AliasMap::const_iterator it = aliases.find(id);
				if( it == aliases.end() ) return "";
				std::stringstream ss;
				BOOST_FOREACH(const pwn::string& n, it->second)
				{
					ss << n << " ";
				}
				return ss.str();
			}

			const pwn::string displayDescription(const pwn::string& id) const
			{
				DescriptionMap::const_iterator it = descriptions.find(id);
				if( it == descriptions.end() ) return "";
				return it->second;
			}

			const pwn::string displayUsage(const pwn::string& name) const
			{
				if( cmd.find(name) != cmd.end() )
				{
					return name;
				}
				else if( cmda.find(name) != cmda.end() )
				{
					return name + " ARG";
				}
				else
				{
					return "";
				}
			}

			void handle(int argc, char* argv[], Main* main)
			{
				argv0 = argv[0];

				for(int i=1; i<argc; ++i)
				{
					try
					{
						if( IsArgument(argv[i]) )
						{
							const std::string name = pwn::core::TrimLeft(argv[i], "-/");
							const std::string val = i+1<argc ? argv[i+1] : "";
							CommandMap::iterator cmdi = cmd.find(name);
							if( cmdi != cmd.end() )
							{
								errors += cmdi->second(main, this);
							}
							else
							{
								CommandArgMap::iterator cmdai = cmda.find(name);
								if( cmdai != cmda.end() )
								{
									errors += cmdai->second(main, this, val);
									++i;
								}
								else
								{
									std::cerr << "Unknown argument " << name << std::endl;
									++errors;
								}
							}
						}
						else
						{
							errors += cmdmain(main, this, argv[i]);
						}
					}
					catch(const std::string& err)
					{
						std::cerr << err << std::endl;
						++errors;
					}
					catch(const char* err)
					{
						std::cerr << err << std::endl;
						++errors;
					}
					catch(...)
					{
						std::cerr << "Unknown error" << std::endl;
						++errors;
					}

					if( stopOnError && errors > 0)
					{
						std::cerr << "Errors encounted, halting.." << std::endl;
						return;
					}

					if( stop )
					{
						return;
					}
				}
			}

			int getErrors() const
			{
				return errors;
			}
		private:
			CommandMap cmd;
			CommandArgMap cmda;
			CommandArg cmdmain;
			AliasMap aliases;
			DescriptionMap descriptions;
			std::vector<pwn::string> commands;
			int errors;
		};

		class Strings
		{
		public:
			Strings& operator<<(const pwn::string& s)
			{
				strings.push_back(s);
				return *this;
			}

			operator std::vector<pwn::string>() const
			{
				return strings;
			}
			std::vector<pwn::string> strings;
		};

		class App
		{
		public:
			ConvertData arg;
			ConvertData old;
			const InputFormat* formatOveride;
			int errors;

			App()
				: formatOveride(0)
				, errors(0)
			{
			}

			void handle(int argc, char* argv[]);
		};

		int CommandArg_Scale(App* app, ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.modelScale = boost::lexical_cast<float>(val);
			app->arg.useModelScale = true;
			return 0;
		}

		int CommandArg_TextureDir(App* app, ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.texturedir = val;
			return 0;
		}

		int CommandArg_AnimDir(App* app, ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.animdir = val;
			return 0;
		}

		int CommandArg_DisplayHelp(App* app, ConsoleArguments<App>* args, const pwn::string& val)
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

		int CommandArg_OutDir(App* app, ConsoleArguments<App>* args, const pwn::string& val)
		{
			app->arg.outdir = val;
			return 0;
		}

		int Command_RunStatistics(App* app, ConsoleArguments<App>* args)
		{
			app->arg.runStatistics = true;
			return 0;
		}

		int Command_MeshInfo(App* app, ConsoleArguments<App>* args)
		{
			app->arg.meshInfo = true;
			return 0;
		}

		int Command_DontWriteResult(App* app, ConsoleArguments<App>* args)
		{
			app->arg.writeResult = false;
			return 0;
		}

		int Command_Verbose(App* app, ConsoleArguments<App>* args)
		{
			app->arg.verbose = true;
			return 0;
		}
		int Command_Restore(App* app, ConsoleArguments<App>* args)
		{
			app->arg = app->old;
			return 0;
		}

		int CommandArg_ForceFormat(App* app, ConsoleArguments<App>* args, const pwn::string& val)
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

		int Command_DontStopOnErrors(App* app, ConsoleArguments<App>* args)
		{
			args->stopOnError = false;
			return 0;
		}

		int Command_DisplayArguments(App* app, ConsoleArguments<App>* args)
		{
			args->displayCommands();
			args->stop = true;
			return 0;
		}

		int CommandArg_RunFile(App* app, ConsoleArguments<App>* args, const pwn::string& file)
		{
			pwn::string animationfile;
			AnimationExtract animations;
			try
			{
				if( SuggestAnimationFile(file, &animationfile) )
				{
					animations = LoadAnimations(animationfile);
				}
				Convert(args->argv0, app->arg, animations, file, app->formatOveride);
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

			return 0;
		}

		void App::handle(int argc, char* argv[])
		{
			ConsoleArguments<App> args;

			args.setArgCommand(Strings() << "scale" << "s", CommandArg_Scale, "scales the loaded mesh");
			args.setArgCommand(Strings() << "texturedir" << "tex" << "t", CommandArg_TextureDir, "sets the texture dir that will be used when loading, @ means meshfilename");
			args.setArgCommand(Strings() << "animdir" << "anim" << "a", CommandArg_AnimDir, "sets the animation dir");
			args.setArgCommand(Strings() << "outdir" << "out" << "o", CommandArg_OutDir, "sets the outdir where to place the result, defaults to meshdir");
			args.setArgCommand(Strings() << "format" << "force" << "f", CommandArg_ForceFormat, "");
			args.setArgCommand(Strings() << "run-file" << "run" << "F", CommandArg_RunFile, "forces a file to run, needed if the path starts with / or -");

			args.setArgCommand(Strings() << "more" << "help-about" << "H", CommandArg_DisplayHelp, "Shows help about a command");

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







