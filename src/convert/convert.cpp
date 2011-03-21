#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

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

namespace InputFormat
{
	enum Type
	{
		Unknown
		, Obj
		, Ms3d_ascii
		, Ms3d_binary
		, Studio3ds
	};
}

const InputFormat::Type SuggestFormat(const pwn::string& ext)
{
	if( ext == ".obj" ) return InputFormat::Obj;
	else if( ext == ".3ds" ) return InputFormat::Studio3ds;
	else if( ext == ".txt" ) return InputFormat::Ms3d_ascii;
	else if( ext == ".ms3d" ) return InputFormat::Ms3d_binary;
	else return InputFormat::Unknown;
}

const InputFormat::Type SuggestFormat(const pwn::string& inputfile, const InputFormat::Type formatOveride)
{
	if( formatOveride != InputFormat::Unknown ) return formatOveride;
	const pwn::string ext = boost::filesystem::path(inputfile).extension();

	return SuggestFormat(ext);
}

const InputFormat::Type SuggestFormatData(const pwn::string& name)
{
	return SuggestFormat("." + name);
}

/**
 Supports: 3ds, obj, milkshape binary & milkshape ascii.
 Add support for x, collada, md2, md3, md5, an8, ogre mesh, dxf & blender
*/
bool Load(pwn::mesh::Builder* builder, pwn::mesh::Animation* animation, const pwn::string& inputfile, const InputFormat::Type formatOveride, bool verbose)
{
	const InputFormat::Type fileFormat = SuggestFormat(inputfile, formatOveride);

	if( verbose ) cout << "reading " << fileFormat << ".." << std::endl;

	switch(fileFormat)
	{
	case InputFormat::Obj:
		{
		WriteDotCallback wdc(verbose);
		pwn::convert::obj::read(builder, inputfile, wdc);
		builder->buildNormals();
		}
		return true;
	case InputFormat::Studio3ds:
		pwn::convert::studio3ds::read(builder, inputfile);
		builder->buildNormals();
		return true;
	case InputFormat::Ms3d_ascii:
		pwn::convert::milkshape::ascii::Read(builder, animation, inputfile);
		builder->buildNormals();
		return true;
	case InputFormat::Ms3d_binary:
		pwn::convert::milkshape::binary::Read(builder, animation, inputfile);
		builder->buildNormals();
		return true;
	default:
		std::cerr << "Unable to determine the kind of reader to use with " << inputfile;
		return false;
	}
}

typedef std::vector<pwn::mesh::AnimationInformation> AnimationExtract;

pwn::string SuggestTextureDirectory(const pwn::string& in)
{
	return boost::filesystem::path(in).replace_extension().filename();
}

pwn::string SuggestAnimationFile(const pwn::string& in)
{
	return boost::filesystem::path(in).replace_extension("animxml").filename();
}

pwn::string SuggestOutDirectory(const pwn::string& in)
{
	return boost::filesystem::path(in).directory_string();
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

bool Convert(const pwn::string& argv0, const ConvertData& cd, const AnimationExtract& animationsToExtract, const pwn::string& inputfile, InputFormat::Type formatOveride)
{
	pwn::string outdir =
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
	read_xml(filename, pt);
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
		InputFormat::Type formatOveride = InputFormat::Unknown;

		for(int i=0; i<argc; ++i)
		{
			if( IsArgument(argv[i]) )
			{
				const std::string name = pwn::core::ToLower(pwn::core::TrimLeft(argv[i], "-/"));
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
					arg.outdir == val;
					++i;
				}
				else if ( name=="s" || name=="stat" || name == "showstat" )
				{
					arg.runStatistics = true;
				}
				else if ( name == "i" || name == "info" )
				{
					arg.meshInfo = true;
				}
				else if( name == "w" || name=="nowrite" )
				{
					arg.writeResults = false;
				}
				else if ( name == "k" || name == "keep" )
				{
					arg = old;
				}
				else if (name == "f" || name=="format" || name=="force" )
				{
					formatOveride = SuggestFormatData(val);
					++i;
				}
			}
			else
			{
				const pwn::string file = argv[i];
				AnimationExtract animations = LoadAnimations(SuggestAnimationFile(file));
				Convert(argv[0], arg, animations, file, formatOveride);
				old = arg;
				arg = ConvertData();
				formatOveride = InputFormat::Unknown;
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







