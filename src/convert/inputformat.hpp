#include "convert.hpp"
#include <pwn/string.h>
#include <vector>

namespace pwn
{
	namespace convert
	{
		class InputFormat
		{
		public:
			virtual ~InputFormat() {}
			virtual const std::string getName() const = 0;
			virtual void load(BuilderList* builders, const std::vector<pwn::string>& subobjects, AnimationList* animation, const pwn::string& inputfile, bool verbose) const = 0;
		};

		const InputFormat* SuggestFormat(const pwn::string& ext);
	}
}
