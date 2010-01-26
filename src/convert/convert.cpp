#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include <pwn/mesh/Mesh>
#include <pwn/meshio/io>

#include "Converter.hpp"
#include "WavefrontObj.hpp"

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

void main(int argc, char* argv[])
{
	namespace po = boost::program_options;
	using namespace std;

	std::string inputfile;
	std::string outdir;

	bool optimize = false;
	bool runStatistics = false;
	bool writeResult = true;
	bool verbose = false;
	bool meshInfo = false;

	pwn::meshio::Compress compress(false);
	
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,?", "produce help message")
		("input,i", po::value<std::string>(&inputfile), "the input file")
		("output,o", po::value<std::string>(&outdir), "the output directory")
		("stats,s", po::value<bool>(&runStatistics)->default_value(false), "1=statistics on input, 0=not")
		("mesh-info,m", po::value<bool>(&meshInfo)->default_value(false), "1=info on input, 0=not")
		("verbose,v", po::value<bool>(&verbose)->default_value(false), "1=detailed information, 0=not")
		("optimize,O", po::value<bool>(&optimize)->default_value(false), "1=optimize mesh, 0=not")
		("compress-materials,M", po::value<bool>(&compress.materials)->default_value(false), "1=write compressed materials, 0=not")
		("compress-normals,N", po::value<bool>(&compress.normals)->default_value(false), "1=write compressed normals, 0=not")
		("compress-positions,P", po::value<bool>(&compress.positions)->default_value(false), "1=write compressed positions, 0=not")
		("compress-texcoords,T", po::value<bool>(&compress.texcoords)->default_value(false), "1=write compressed texture coordinates, 0=not")
		("write,w", po::value<bool>(&writeResult)->default_value(true), "1=write out file, 0=don't")
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

	if (vm.count("help"))
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

		if( verbose ) cout << "writing.." << endl;
		if( writeResult ) pwn::meshio::Write(mesh, (boost::filesystem::path(outdir) / boost::filesystem::path(inputfile).filename()).replace_extension("mesh").string(), compress);

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