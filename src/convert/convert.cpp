#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include <pwn/mesh/mesh.h>
#include <pwn/meshio/io.h>
#include <pwn/mesh/builder.h>
#include <boost/foreach.hpp>

#include "Converter.hpp"
#include "WavefrontObj.hpp"
#include "3ds.hpp"

#include "MilkshapeAscii.hpp"
#include "MilkshapeBinary.hpp"

#pragma comment (lib, "physfs.lib")

using namespace std;

std::ostream& operator<<(std::ostream& os, const ::pwn::convert::Stat& s)
{
	os << "< " << s.min << " | " << s.average << " | " << s.max << " >";
	return os;
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

const pwn::string SuggestFormat(const pwn::string& inputfile, const pwn::string& formatOveride)
{
	if( formatOveride != "" ) return formatOveride;
	const pwn::string ext = boost::filesystem::path(inputfile).extension();

	if( ext == ".obj" ) return "obj";
	else if( ext == ".3ds" ) return "3ds";
	else if( ext == ".txt" ) return "ms3d-ascii";
	else if( ext == ".ms3d" ) return "ms3d-binary";
	else return "";
}

bool Load(pwn::convert::OptimizedMeshBuilder& builder, const pwn::string& inputfile, const pwn::string& formatOveride, bool verbose)
{
	const pwn::string fileFormat = SuggestFormat(inputfile, formatOveride);

	if( fileFormat == "" )
	{
		std::cerr << "Unable to determine the kind of reader to use with " << inputfile;
		return false;
	}

	if( verbose ) cout << "reading " << fileFormat << ".." << std::endl;

	if( fileFormat == "obj" )
	{
		WriteDotCallback wdc(verbose);
		pwn::convert::obj::read(&builder, inputfile, wdc);
		return true;
	}
	else if( fileFormat == "3ds" )
	{
		pwn::convert::studio3ds::read(&builder, inputfile);
		return true;
	}
	else if (fileFormat == "ms3d-ascii" )
	{
		pwn::convert::milkshape::ascii::Read(&builder, inputfile);
		return true;
	}
	else if ( fileFormat == "ms3d-binary" )
	{
		pwn::convert::milkshape::binary::Read(&builder, inputfile);
		return true;
	}
	else
	{
		std::cerr << fileFormat << " is not a recognzied format... " << endl;
		return false;
	}
}

struct ConvertMesh
{
	ConvertMesh(const pwn::string& in)
		: inputfile(in)
		, formatOveride("")
		, useModelScale(false)
		, modelScale(1)
		, texturedir(boost::filesystem::path(in).replace_extension().filename())
		, animdir("")
		, moutdir(boost::filesystem::path(in).directory_string())
	{
	}

	pwn::string inputfile;
	pwn::string formatOveride;
	bool useModelScale;
	float modelScale;
	pwn::string texturedir;
	pwn::string animdir;
	pwn::string moutdir;
	std::vector<pwn::mesh::AnimationInformation> animationsToExtract;

	bool run(const pwn::string& aoutdir, bool runStatistics, bool verbose, bool meshInfo, bool writeResult) const
	{
		const pwn::string outdir = outdir.empty() ? moutdir : aoutdir;
		try
		{
			pwn::convert::OptimizedMeshBuilder builder(false);
			if( Load(builder, inputfile, formatOveride, verbose) == false ) return false;

			pwn::mesh::Flatouter flatouter;

			pwn::mesh::Mesh mesh;
			builder.mBuilder.makeMesh(mesh, &flatouter);

			pwn::mesh::Animation& animation = builder.mAnimation;
			flatouter.modify(&animation);

			{
				pwn::mesh::Pose p;
				animation.getPose(3.5f, &p);
				pwn::mesh::CompiledPose cp(p, mesh.bones);
			}

			const pwn::uint32 validationErrors = mesh.validate();
			if( validationErrors != 0)
			{
				cerr << inputfile << " failed validation with " << validationErrors << " error(s)... ignoring file..." << endl;
				return false;
			}

			if( useModelScale )
			{
				if( verbose ) cout << "scaling " << modelScale << ".." << std::endl;
				pwn::mesh::Scale(&mesh, modelScale);
				animation.scale(modelScale);
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
					<< " materials: " << mesh.triangles.size() << endl
					<< " triangles: " << pwn::mesh::NumberOfTriangles(mesh) << endl
					<< endl;
			}

			if( writeResult )
			{
				if( verbose ) cout << "writing.." << endl;
				pwn::meshio::WriteTarget wt(outdir);
				pwn::meshio::Write(mesh, boost::filesystem::path(inputfile).replace_extension("mesh").filename());
			}

			BOOST_FOREACH(const pwn::mesh::AnimationInformation& ai, animationsToExtract)
			{
				pwn::mesh::Animation ani;
				animation.subanim(ai, &ani);

				pwn::string adir = animdir;
				if( adir.empty() )
				{
					adir = (
							boost::filesystem::path(outdir)
							/ boost::filesystem::path(inputfile).replace_extension().filename()
							)
						.directory_string();
				};

				pwn::meshio::WriteTarget wt(adir);
				pwn::meshio::Write(ani, boost::filesystem::path(ai.name).replace_extension("anim").filename());
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

			return true;
		}
		catch(const pwn::string& str)
		{
			cerr << endl << str << endl;
			return false;
		}
		catch(const char* str)
		{
			cerr << endl << str << endl;
			return false;
		}
		catch(...)
		{
			cerr << endl << "general failue." << endl;
			return false;
		}
	}
};

void RunXml(const pwn::string& filename)
{
	using boost::property_tree::ptree;

	try
	{
		ptree pt;
		read_xml(filename, pt);

		pwn::string outdir  = pt.get<pwn::string>("convert.outdir");
		bool runstats = pt.get("convert.runstats", false);
		bool verbose = pt.get("convert.verbose", false);
		bool displayMeshInfo = pt.get("convert.displayMeshInfo", false);
		bool writeResults = pt.get("convert.writeResults", true);

		std::vector<ConvertMesh> cm;

		BOOST_FOREACH(ptree::value_type &v, pt.get_child("convert.sources"))
		{
			ptree source = v.second;
			const pwn::string inputfile = source.get<pwn::string>("file");
			ConvertMesh cmesh(inputfile);

			boost::optional<pwn::real> scale = source.get_optional<pwn::real>("scale");
			if( scale.is_initialized() )
			{
				cmesh.modelScale = scale.get();
				cmesh.useModelScale = true;
			}

			BOOST_FOREACH(ptree::value_type &an, source.get_child("animations"))
			{
				ptree anida = an.second;
				const pwn::string name = anida.get<pwn::string>("name");
				const pwn::uint32 start = anida.get<pwn::uint32>("start");
				const pwn::uint32 end = anida.get<pwn::uint32>("end");
				cmesh.animationsToExtract.push_back(pwn::mesh::AnimationInformation(start, end, name));
			}

			cm.push_back(cmesh);
		}

		BOOST_FOREACH(const ConvertMesh& cmesh, cm)
		{
			cmesh.run(outdir, runstats, verbose, displayMeshInfo, writeResults);
		}
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}
}

void main(int argc, char* argv[])
{
	using namespace std;

	const pwn::string filename(argv[1]);
	RunXml(filename);
}







