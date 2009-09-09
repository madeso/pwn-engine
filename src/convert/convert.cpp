#include <boost/program_options.hpp>
#include <string>
#include <iostream>

#include "Converter.hpp"

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
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

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
		try
		{
			pwn::convert::Converter con;
			pwn::convert::obj::read(&con, inputfile);
			cout << "done." << endl;
		}
		catch(...)
		{
			cerr << "failed." << endl;
		}
	}
}