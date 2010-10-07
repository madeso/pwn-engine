#ifndef PWN_RENDER_FSE_PROVIDER
#define PWN_RENDER_FSE_PROVIDER

#include <pwn/render/fse/RenderArgs.h>
#include <boost/utility.hpp>
#include <vector>
#include <pwn/core/config.h>
#include <pwn/render/fse/types.h>
#include <pwn/render/shader.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class Link
				: boost::noncopyable
			{
			public:
				Link(const string& name);
				~Link();

				void provide(WorldPtr world, RenderArgs ra);
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
				void provide(WorldPtr world, const RenderArgs& ra);
				std::vector<Provider*>& getProviders();
				void link(Linker* linker);
				void bind(Binder* bd);
				string toString() const;
				
			public:
				TargetPtr getTarget();
				BufferReferencePtr createBuffer(const string& name);
				void postlink(Linker* linker);

				ShaderPtr getShader();

			protected:
				void denyAutocallOfCommands();
				void callCommands();
				Provider(const core::Ptree& el);
				virtual ~Provider();

				const core::Ptree shaderdata;
				ShaderPtr shader;
				
				virtual void doLink(Linker* linker) = 0;
				virtual void doBind(Binder* bd) = 0;
			public:
				virtual void doProvide(WorldPtr world, const RenderArgs& ra) = 0;
				
			private:
				TargetPtr target;
				const string targetname;
				string id;
				bool autocallCommands;
				std::vector<CommandPtr> commands;
				std::vector<Provider*> providers;
			};
		}
	}
}

#endif
