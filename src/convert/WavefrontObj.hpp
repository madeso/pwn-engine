#ifndef PWN_CONVERT_WAVEFRONT_OBJ_HPP
#define PWN_CONVERT_WAVEFRONT_OBJ_HPP

#include <string>

namespace pwn
{
	namespace convert
	{
		class OptimizedMeshBuilder;
		namespace obj
		{
			struct VoidVoidCallback
			{
				virtual void perform() = 0;
			};
			void read(OptimizedMeshBuilder* builder, const std::string& file, VoidVoidCallback& cb);
		}
	}
}

#endif
