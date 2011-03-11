#ifndef PWN_CONVERT_WAVEFRONT_OBJ_HPP
#define PWN_CONVERT_WAVEFRONT_OBJ_HPP

#include <string>

namespace pwn
{
	namespace mesh
	{
		class Builder;
	}

	namespace convert
	{
		namespace obj
		{
			struct VoidVoidCallback
			{
				virtual void perform() = 0;
			};
			void read(pwn::mesh::Builder* builder, const std::string& file, VoidVoidCallback& cb);
		}
	}
}

#endif
