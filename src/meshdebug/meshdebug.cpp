#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

#include <pwn/mesh/mesh.h>
#include <pwn/io/io.h>
#include <pwn/assert.h>
#include <pwn/core/vec.h>
#include <pwn/core/stringutils.h>
#include <pwn/core/consolearguments.h>
#include <fstream>

using namespace std;

namespace pwn
{
	namespace meshdebug
	{
		class App
		{
		public:
			int errors;
			
			App()
				: errors(0)
			{
			}

			void handle(int argc, char* argv[]);
		};

		template<typename T>
		T& operator<<(T& t, const math::Rgba& r)
		{
			t << "(" << r.r << " " << r.g << " " << r.b << " " << r.a << ")";
			return t;
		}

		void DumpData(const mesh::Mesh& mesh, pwn::string target)
		{
			std::ofstream f(target.c_str());
			if( f.good() == false ) throw "Failed to open outfile";

			const mesh::VertexData& d = mesh.data();
			f << d.getCount() << " points" << std::endl;
			for(int i=0; i<d.getCount(); ++i)
			{
				mesh::Point p = d.getPoint(i);
				f << i << std::endl << p.location << std::endl << p.normal << std::endl << p.textcoord << std::endl << p.bone << std::endl << std::endl;
			}

			f << "--------------------------------------------------------------------------------------" << std::endl << std::endl;

			f << "bones" << std::endl;
			BOOST_FOREACH(const pwn::mesh::Bone& b, mesh.getBones())
			{
				f << b.name << std::endl
					<< b.rot << std::endl;
				if( b.hasParent() )
					f << b.getParent();
				else 
					f << "no parent";
				f << std::endl << std::endl;
			}

			f << "--------------------------------------------------------------------------------------" << std::endl << std::endl;

			f << "Faces: " << std::endl;
			BOOST_FOREACH(const pwn::mesh::Mesh::TriangleMap::value_type& tm, mesh.getTriangles())
			{
				f << "using material: " << tm.first << std::endl
					<< "---------------------------" << std::endl;
				BOOST_FOREACH(const pwn::mesh::Triangle& t, tm.second)
				{
					f << t.v0 << std::endl
						<< t.v1 << std::endl
						<< t.v2 << std::endl
						<< std::endl;
				}
			}

			f << "--------------------------------------------------------------------------------------" << std::endl << std::endl;

			f << "Materials: " << std::endl;
			BOOST_FOREACH(const pwn::mesh::Material& m, mesh.getMaterials())
			{
				f << m.ambient << std::endl << m.diffuse << std::endl << m.specular << std::endl << m.emission << std::endl << m.shininess << std::endl << m.texture_diffuse << std::endl << std::endl;
			}
		}

		pwn::string GetAbsolutePath(const pwn::string& in)
		{
			return boost::filesystem::system_complete( boost::filesystem::path( in.c_str(), boost::filesystem::native ) ).string();
		}

		int CommandArg_RunFile(App* app, core::ConsoleArguments<App>* args, const pwn::string& afile)
		{
			mesh::Mesh mesh;
			const pwn::string file = GetAbsolutePath(afile);

			{
				const pwn::string fdir = boost::filesystem::path(file).remove_filename().directory_string();
				const pwn::string fname = boost::filesystem::path(file).filename();
				io::WriteTarget wt( GetAbsolutePath(args->argv0), fdir);
				io::Read(&mesh, fname);
			}

			DumpData(mesh, boost::filesystem::path(file).replace_extension("txt").string());
			return 0;
		}

		void App::handle(int argc, char* argv[])
		{
			core::ConsoleArguments<App> args;

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
		cout << "error usage: pwn-meshdebug arguments filename";
		return 1;
	}

	pwn::meshdebug::App a;
	a.handle(argc, argv);

	return a.errors;
}

