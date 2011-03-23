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

#include "MilkshapeAscii.hpp"
#include "MilkshapeBinary.hpp"
#include <pwn/assert.h>
#include <pwn/core/stringutils.h>
#include <boost/lexical_cast.hpp>

using namespace std;

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
	virtual void load(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const = 0;
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
	void load(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
	{
		WriteDotCallback wdc(verbose);
		pwn::convert::obj::read(builder, inputfile, wdc);
		builder->buildNormals();
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
	void load(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
	{
		pwn::convert::studio3ds::read(builder, inputfile);
		builder->buildNormals();
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
	void load(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
	{
		pwn::convert::milkshape::ascii::Read(builder, animation, inputfile);
		builder->buildNormals();
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
	void load(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const pwn::string& inputfile, bool verbose) const
	{
		pwn::convert::milkshape::binary::Read(builder, animation, inputfile);
		builder->buildNormals();
	}
};

const InputFormat* SuggestFormat(const pwn::string& ext)
{
	if( ext == ".obj" ) return GetInputFormat<InputFormat_Obj>();
	else if( ext == ".3ds" ) return GetInputFormat<InputFormat_Studio3ds>();
	else if( ext == ".txt" ) return GetInputFormat<InputFormat_Ms3d_ascii>();
	else if( ext == ".ms3d" ) return GetInputFormat<InputFormat_Ms3d_binary>();
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
bool Load(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const pwn::string& inputfile, const InputFormat* formatOveride, bool verbose)
{
	const InputFormat* fileFormat = SuggestFormat(inputfile, formatOveride);

	if(fileFormat)
	{
		if( verbose ) cout << "reading " << inputfile << " (" << fileFormat->getName() << ").." << std::endl;
		fileFormat->load(builder, animation, inputfile, verbose);
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
	pwn::mesh::Builder builder;
	pwn::mesh::Animation animation;
	if( Load(&builder, &animation, inputfile, formatOveride, cd.verbose) == false ) return false;

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

	pwn::mesh::MoveTextures(&builder, cd.texturedir);

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
			<< "Mesh information: " << endl
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
	return str[0] == '-' || str[1] == '/';
}

class app
{
public:
	app()
		: errors(0)
	{
	}

	void handle(int argc, char* argv[])
	{
		ConvertData arg;
		ConvertData old;
		const InputFormat* formatOveride = 0;
		bool breakOnErrors = true;

		for(int i=1; i<argc; ++i)
		{
			if( IsArgument(argv[i]) )
			{
				const std::string name = pwn::core::TrimLeft(argv[i], "-/");
				const std::string val = i+1<argc ? argv[i+1] : "";
				if( name == "s" || name == "scale" )
				{
					arg.modelScale = boost::lexical_cast<float>(val);
					arg.useModelScale = true;
					++i;
				}
				else if ( name == "t" || name=="tex" || name == "texturedir" )
				{
					arg.texturedir = val;
					++i;
				}
				else if (name == "a" || name=="anim" || name == "animdir" )
				{
					arg.animdir = val;
					++i;
				}
				else if ( name == "o" || name=="out" || name == "outdir" )
				{
					arg.outdir = val;
					++i;
				}
				else if ( name=="S" || name=="stat" || name == "showstat" )
				{
					arg.runStatistics = true;
				}
				else if ( name == "i" || name == "info" )
				{
					arg.meshInfo = true;
				}
				else if( name == "w" || name=="nowrite" )
				{
					arg.writeResult = false;
				}
				else if( name == "v" || name=="verbose" )
				{
					arg.verbose = true;
				}
				else if ( name == "k" || name == "keep" )
				{
					arg = old;
				}
				else if (name == "f" || name=="format" || name=="force" )
				{
					++i;
					const InputFormat* sf = SuggestFormatData(val);
					if( sf )
					{
						formatOveride = sf;
					}
					else
					{
						std::cerr << "Unknown format override: " << val << " using last override";
						++errors;
					}
				}
				else if (name == "B" || name=="no-break" || name=="no-break-on-error" )
				{
					breakOnErrors = false;
				}
				else
				{
					std::cerr << "Unknown option " << name << std::endl;
					++errors;
				}
			}
			else
			{
				const pwn::string file = argv[i];
				pwn::string animationfile;
				AnimationExtract animations;
				try {
					if( SuggestAnimationFile(file, &animationfile) )
					{
						animations = LoadAnimations(animationfile);
					}
					Convert(argv[0], arg, animations, file, formatOveride);
				}
				catch(const std::exception& ex)
				{
					std::cerr << "Error: " << ex.what() << std::endl;
					++errors;
				}
				catch(const char* msg)
				{
					std::cerr << "Error: " << msg << std::endl;
					++errors;
				}
				catch(const std::string& msg)
				{
					std::cerr << "Error: " << msg << std::endl;
					++errors;
				}
				catch(...)
				{
					std::cerr << "Unknown error detected!" << std::endl;
					++errors;
				}
				old = arg;
				arg = ConvertData();
				formatOveride = 0;
			}

			if( breakOnErrors && errors > 0 )
			{
				std::cerr << "Errors detected, aborting" << std::endl;
				return;
			}
		}
	}

	int errors;
};

int main(int argc, char* argv[])
{
	using namespace std;

	if( argc == 1 )
	{
		cout << "error usage: pwn-convert filename arguments";
		return 1;
	}

	app a;
	a.handle(argc, argv);

	return a.errors;
}







