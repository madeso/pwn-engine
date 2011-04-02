#ifndef PWN_CONVERT_WAVEFRONT_OBJ_HPP
#define PWN_CONVERT_WAVEFRONT_OBJ_HPP

#include <string>
#include "convert.hpp"

namespace pwn
{
	namespace convert
	{
		namespace obj
		{
			struct VoidVoidCallback
			{
				virtual void perform() = 0;
			};
			void read(BuilderList* builders, const std::string& file, VoidVoidCallback& cb);
		}
	}
}

#endif
