#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include <pwn/mesh/Mesh>
#include <pwn/meshio/io>
#include <pwn/mesh/builder>

#include "Converter.hpp"
#include "WavefrontObj.hpp"

#pragma comment (lib, "physfs.lib")

std::ostream& operator<<(std::ostream& os, const ::pwn::convert::Stat& s)
{
	os << "< " << s.min << " | " << s.average << " | " << s.max << " >";
	return os;
}

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

struct Cmd
{
	Cmd(const pwn::string& aLongCmd, const pwn::tchar aShortCmd)
		: longCmd(aLongCmd)
		, shortCmd(aShortCmd)
		, combined(aLongCmd+ "," + aShortCmd)
	{
	}

	operator const char*() const
	{
		return combined.c_str();
	}

	bool eval(const boost::program_options::variables_map& vm) const
	{
		const std::size_t c = vm.count(longCmd.c_str());
		const bool r = c != 0;
		//std::cout << longCmd << ": " << c << "/" << r << std::endl;
		return r;
	}

	pwn::string longCmd;
	pwn::tchar shortCmd;
	pwn::string combined;
};

void main(int argc, char* argv[])
{
	namespace po = boost::program_options;
	using namespace std;

	pwn::string inputfile;
	pwn::string outdir;
	pwn::string texturedir;

	const Cmd Help					("help",				'?');
	const Cmd Input					("input",				'i');
	const Cmd Output				("output",				'o');
	const Cmd Stats					("stats",				's');
	const Cmd MeshInfo				("show-mesh-info",		'm');
	const Cmd NotVerbose			("not-verbose",			'v');
	const Cmd Optimize				("optimize",			'O');
	const Cmd CompressMaterials		("compress-materials",	'M');
	const Cmd CompressNormals		("compress-normals",	'N');
	const Cmd CompressPositions		("compress-positions",	'P');
	const Cmd CompressTexcoords		("compress-texcoords",	'T');
	const Cmd DontWrite				("dont-write",			'w');
	const Cmd MoveTextures			("move-textures",		't');
	
	po::options_description desc("Allowed options");
	desc.add_options()
		(Help, "produce help message")
		(Input, po::value<pwn::string>(&inputfile), "the input file")
		(Output, po::value<pwn::string>(&outdir), "the output directory")
		(MoveTextures, po::value<pwn::string>(&texturedir),	"Move the textures")
		(Stats,					"Write optimization statistics")
		(MeshInfo,				"Write information about mesh")
		(NotVerbose,			"Silent/not verbose output")
		(Optimize,				"Optimize mesh")
		(CompressMaterials,		"Write compressed materials")
		(CompressNormals,		"Write compressed normals")
		(CompressPositions,		"Write compressed positions")
		(CompressTexcoords,		"Write compressed texture coordinates")
		(DontWrite,				"Don't write out file")
		;

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error while parsing commandline: " << e.what() << std::endl;
		return;
	}

	bool optimize = Optimize.eval(vm);
	bool runStatistics = Stats.eval(vm);
	bool writeResult = !DontWrite.eval(vm);
	bool verbose = !NotVerbose.eval(vm);
	bool meshInfo = MeshInfo.eval(vm);

	pwn::meshio::Compress compress(false);
	compress.materials = CompressMaterials.eval(vm);
	compress.normals = CompressNormals.eval(vm);
	compress.positions = CompressPositions.eval(vm);
	compress.texcoords = CompressTexcoords.eval(vm);

	if (Help.eval(vm))
	{
		cerr << desc << endl;
		return;
	}
	if ( inputfile == "" )
	{
		cerr << "Error: input not set" << endl << endl;
		cerr << desc << endl;
		return;
	}
	if ( writeResult==true && outdir == "" )
	{
		cerr << "Error: output not set" << endl << endl;
		cerr << desc << endl;
		return;
	}
	
	try
	{
		const bool optimizeNormals = optimize && compress.normals;
		pwn::mesh::Mesh mesh;
		if( verbose && optimizeNormals ) cout << "optimizing normals ACTIVE.." << endl;
		pwn::convert::OptimizedMeshBuilder builder(&mesh, optimizeNormals);
		if( verbose ) cout << "reading..";

		/* scope */ {
			WriteDotCallback wdc(verbose);
			pwn::convert::obj::read(&builder, inputfile, wdc);
		}

		if( verbose ) cout << endl;

		pwn::mesh::MoveTextures(&mesh, texturedir);

		if( meshInfo )
		{
			cout
				<< "Mesh information: " << endl
				<< " positions: " << mesh.positions.size() << endl
				<< " normals: " << mesh.normals.size() << endl
				<< " texcoords: " << mesh.texcoords.size() << endl
				<< " triangles: " << mesh.triangles.size() << endl
				<< endl;
		}

		if( writeResult )
		{
			if( verbose ) cout << "writing.." << endl;
			pwn::meshio::WriteTarget wt(outdir);
			pwn::meshio::Write(mesh, boost::filesystem::path(inputfile).replace_extension("mesh").filename(), compress);
		}

		if( runStatistics )
		{
			if( verbose ) cout << "testing..." << endl;

			pwn::convert::Stat positions;
			pwn::convert::Stat normals;

			pwn::convert::EstimatedDataLossWhenCompressing(mesh, &positions, &normals);

			cout
				<< "Estimated loss < min | avg | max > " << endl
				<< " for positions: " << positions << endl
				<< " for normals: " << normals << endl
				<< endl
				<< "Saving was: " << endl
				<< builder.removedNormals() * 100 << "% normals removed" << endl
				<< " wich is " << builder.numberOfRemovedNormals() << "!" << endl;

		}

		if( verbose ) cout << endl << "done." << endl;
	}
	catch(const pwn::string& str)
	{
		cerr << endl << str << endl;
	}
	catch(const char* str)
	{
		cerr << endl << str << endl;
	}
	catch(...)
	{
		cerr << endl << "general failue." << endl;
	}
}