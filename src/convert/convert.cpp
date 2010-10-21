#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include <pwn/mesh/mesh.h>
#include <pwn/io/io.h>
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

const InputFormat::Type SuggestFormat(const pwn::string& inputfile, const InputFormat::Type formatOveride)
{
	if( formatOveride != InputFormat::Unknown ) return formatOveride;
	const pwn::string ext = boost::filesystem::path(inputfile).extension();

	if( ext == ".obj" ) return InputFormat::Obj;
	else if( ext == ".3ds" ) return InputFormat::Studio3ds;
	else if( ext == ".txt" ) return InputFormat::Ms3d_ascii;
	else if( ext == ".ms3d" ) return InputFormat::Ms3d_binary;
	else return InputFormat::Unknown;
}

/**
 Supports: 3ds, obj, milkshape binary & milkshape ascii.
 Add support for x, collada, md2, md3, md5, an8, ogre mesh, dxf & blender
*/
bool Load(pwn::convert::OptimizedMeshBuilder& builder, const pwn::string& inputfile, const InputFormat::Type formatOveride, bool verbose)
{
	const InputFormat::Type fileFormat = SuggestFormat(inputfile, formatOveride);

	if( verbose ) cout << "reading " << fileFormat << ".." << std::endl;

	switch(fileFormat)
	{
	case InputFormat::Obj:
		{
		WriteDotCallback wdc(verbose);
		pwn::convert::obj::read(&builder, inputfile, wdc);
		}
		return true;
	case InputFormat::Studio3ds:
		pwn::convert::studio3ds::read(&builder, inputfile);
		return true;
	case InputFormat::Ms3d_ascii:
		pwn::convert::milkshape::ascii::Read(&builder, inputfile);
		return true;
	case InputFormat::Ms3d_binary:
		pwn::convert::milkshape::binary::Read(&builder, inputfile);
		return true;
	default:
		std::cerr << "Unable to determine the kind of reader to use with " << inputfile;
		return false;
	}
}

struct ConvertMesh
{
	ConvertMesh(const pwn::string& in)
		: inputfile(in)
		, formatOveride(InputFormat::Unknown)
		, useModelScale(false)
		, modelScale(1)
		, texturedir(boost::filesystem::path(in).replace_extension().filename())
		, animdir("")
		, moutdir(boost::filesystem::path(in).directory_string())
	{
	}

	pwn::string inputfile;
	InputFormat::Type formatOveride;
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
				pwn::io::WriteTarget wt(outdir);
				pwn::io::Write(mesh, boost::filesystem::path(inputfile).replace_extension("mesh").filename());
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

				pwn::io::WriteTarget wt(adir);
				pwn::io::Write(ani, boost::filesystem::path(ai.name).replace_extension("anim").filename());
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







