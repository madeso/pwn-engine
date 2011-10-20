#ifndef PWN_RENDER_FSE_PROVIDER
#define PWN_RENDER_FSE_PROVIDER

#include <pwn/render/RenderArgs.h>
#include <boost/utility.hpp>
#include <vector>
#include <pwn/core/config.h>
#include <pwn/render/fse/types.h>
#include <pwn/render/shader.h>

namespace pwn
{
	namespace render
	{
		class ShaderPool;

		namespace fse
		{
			class Link
				: boost::noncopyable
			{
			public:
				Link(const string& name);
				~Link();

				void provide(World3* world, RenderArgs ra);
				void sortout(LinkerPtr usr);

			private:
				ProviderPtr prov;
				const string name;
			};

			class Provider
			{
			public:
				void setId(const string& id);
				const string getId() const;
				void provide(World3* world, const RenderArgs& ra);
				std::vector<Provider*>& getProviders();
				void link(Linker* linker);
				void bind(Binder* bd, ShaderPool* shaders);
				string toString() const;

			public:
				TargetPtr getTarget();
				BufferReferencePtr createBuffer(const string& name);
				void postlink(Linker* linker);

				ShaderPtr getShader();

			protected:
				void denyAutocallOfCommands();
				void callCommands();
				Provider(const core::Ptree& el, const string& file);
				virtual ~Provider();

				const core::Ptree shaderdata;
				ShaderPtr shader;

				virtual void doLink(Linker* linker) = 0;
				virtual void doBind(Binder* bd) = 0;
			public:
				virtual void doProvide(World3* world, const RenderArgs& ra) = 0;

			private:
				TargetPtr target;
				const string targetname;
				string id;
				string file;
				bool autocallCommands;
				std::vector<CommandPtr> commands;
				std::vector<Provider*> providers;
			};
		}
	}
}

#endif
