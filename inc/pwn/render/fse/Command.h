#ifndef PWN_RENDER_FSE_COMMAND
#define PWN_RENDER_FSE_COMMAND

#include <pwn/core/config.h>
#include <pwn/render/fse/types.h>
#include <vector>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			class Command
			{
			public:
				Command(const core::Ptree& el, Provider* prov);
				virtual ~Command();

				virtual void apply() = 0;

				// should be called by our provider
				void link(Linker* linker);
				void bind(Binder* b);

				virtual std::vector<Provider*> getDependencies() = 0;

				const string toString() const;
			protected:
				BufferReferencePtr createBuffer(const string& name);

				virtual void doLink(Linker* user) = 0;
				virtual void doBind(Binder* bd) = 0;

				static const std::vector<Provider*> NoDependencies;
			private:
				const string id;
				Provider* prov;
			};

			template<typename T>
			T& operator<<(T& t, CommandPtr c)
			{
				if(c.get())
				{
					t << c->toString();
				}
				else
				{
					t << c;
				}
				return t;
			}
		}
	}
}

#endif
