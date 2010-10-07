#ifndef PWN_RENDER_FSE_TARGET
#define PWN_RENDER_FSE_TARGET

#include <pwn/render/fse/types.h>
#include <vector>
#include <map>
#include <pwn/render/size.h>

namespace pwn
{
	namespace render
	{
		class RenderArgs;

		namespace fse
		{
			class Target
			{
			public:
				Target();
				virtual ~Target();

				virtual void link(Linker* usr) = 0;
				virtual void apply(Provider& a, World3* world, const RenderArgs& ra) = 0; // calls doProvide on provider
				virtual int getWidth() = 0;
				virtual int getHeight() = 0;
				const string& getId() const;
				void setId(const string& id);
				Provider* getProvider();
				void setProvider(Provider* provider);
				const string toString() const;
				
			public:
				void bind(Binder* binder);
				
			protected:
				BufferReferencePtr createBuffer(const string& name);
				BufferReferencePtr createBuffer(const string& name, int width, int height);
				
			private:
				void associate(const string& name, int width, int height);
				
				string id;
				std::vector<BufferReferencePtr> references;
				typedef std::map<string, Size> AssMap;
				AssMap associations;
				Provider* provider;
			};
		}
	}
}

#endif
