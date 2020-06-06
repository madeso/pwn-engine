#include "inputformat.hpp"

#include "WavefrontObj.hpp"
#include "3ds.hpp"
#include "An8.hpp"
#include "MilkshapeAscii.hpp"
#include "MilkshapeBinary.hpp"

namespace pwn
{
	namespace convert
	{
		template<class IF>
		static InputFormat* GetInputFormat()
		{
			static IF format;
			return &format;
		};

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
				if(verbose)
				{
					std::cout << ".";
				}
			}
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
			void load(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& inputfile, bool verbose) const
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
			void load(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& inputfile, bool verbose) const
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
			void load(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& inputfile, bool verbose) const
			{
				pwn::convert::milkshape::ascii::Read(builders, inputfile);
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
			void load(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& inputfile, bool verbose) const
			{
				pwn::convert::an8::read(builders, subobjects, inputfile);
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
			void load(BuilderList* builders, const std::vector<pwn::string>& subobjects, const pwn::string& inputfile, bool verbose) const
			{
				pwn::convert::milkshape::binary::Read(builders, inputfile);
			}
		};

		const InputFormat* SuggestFormat(const pwn::string& ext)
		{
			if(ext == ".obj")
			{
				return GetInputFormat<InputFormat_Obj>();
			}
			else if(ext == ".3ds")
			{
				return GetInputFormat<InputFormat_Studio3ds>();
			}
			else if(ext == ".txt")
			{
				return GetInputFormat<InputFormat_Ms3d_ascii>();
			}
			else if(ext == ".ms3d")
			{
				return GetInputFormat<InputFormat_Ms3d_binary>();
			}
			else if(ext == ".an8")
			{
				return GetInputFormat<InputFormat_An8>();
			}
			else
			{
				return 0;
			}
		}
	}
}
