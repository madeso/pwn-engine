#ifndef PWN_RENDER_FSE_BINDER
#define PWN_RENDER_FSE_BINDER

#include <boost/shared_ptr.hpp>
#include <pwn/string.h>
#include <pwn/render/fse/types.h>
#include <pwn/core/map.h>
#include <set>
#include <pwn/core/pool.h>
#include <pwn/render/shader.h>
#include <pwn/render/size.h>

namespace pwn
{
	namespace render
	{
		class ShaderPool;

		namespace fse
		{
			class FboPool
			{
			public:
				FboPool(int width, int height);
				~FboPool();

				FboPtr create(const string& name);
				void release(FboPtr fbo);
			private:
				core::Pool<FboPtr> pool;
				core::Map<FboPtr> map;
			};
			
			class Binder
			{
			public:
				Binder(Linker* linker, ShaderPool* pool);
				~Binder();

				string toString() const;
			public:
				ShaderPtr getShader(const string& shadername);
				ShaderPtr getShaderOrNull(const string& shadername);
				void reference(BufferReferencePtr br);
				
				void createBuffers();
				void associate(const string& p, Size size);

			private:
				FboPtr allocate(const string& name);

				FboCreatorPtr getCreator(const string& name);
				FboCreatorPtr getCreator(Size size);
				void release(FboPtr buff);

				Size sizeOf(const string& name) const;
			private:
				core::Map<ShaderPtr> shaders;
				std::set<BufferReferencePtr> references;
				typedef std::map<string, Size> AssociationMap;
				AssociationMap associations;
				typedef std::map<Size, FboCreatorPtr > CreatorMap;
				CreatorMap creators;
			};
		}
	}
}

#endif
