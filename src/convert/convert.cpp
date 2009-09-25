#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include "Converter.hpp"
#include "Writer.hpp"

#include "WavefrontObj.hpp"

void main(int argc, char* argv[])
{
	namespace po = boost::program_options;
	using namespace std;

	std::string inputfile;
	std::string outdir;
	
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,?", "produce help message")
		("input,i", po::value<std::string>(&inputfile), "the input file")
		("output,o", po::value<std::string>(&outdir), "the output directory")
		("optimize", "optimize result")
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
	}
	else if ( inputfile == "" )
	{
		cerr << "Error: input not set" << endl << endl;
		cerr << desc << endl;
	}
	else if ( outdir == "" )
	{
		cerr << "Error: output not set" << endl << endl;
		cerr << desc << endl;
	}
	else
	{
		const bool optimize = vm.count("optimize") > 0;
		try
		{
			pwn::convert::Converter con;
			pwn::convert::obj::read(&con, inputfile);
			pwn::convert::Write(con, (boost::filesystem::path(outdir) / boost::filesystem::path(inputfile).filename()).replace_extension("mesh").string(), optimize);
			pwn::convert::Test(con);
			cout << "done." << endl;
			std::cin.get();
		}
		catch(...)
		{
			cerr << "failed." << endl;
		}
	}
}