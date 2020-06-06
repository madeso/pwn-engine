#include <pwn/render/fse/Linker.h>
#include <pwn/render/fse/exceptions.h>
#include <pwn/render/fse/Provider.h>
#include <pwn/render/fse/Target.h>
#include <pwn/render/fse/Pipeline.h>
#include <boost/foreach.hpp>

#include <boost/property_tree/xml_parser.hpp>
#include <pwn/render/fse/Target_Targets.h>
#include <pwn/render/fse/Provider_Providers.h>
#include <pwn/render/fse/Provider.h>
#include <pwn/render/shaderpool.h>

#include <pwn/io/config.h>
#include <pwn/core/str.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			namespace // local
			{
				TargetPtr CreateTargetFunction(const string& name)
				{
					throw FseException(name + " is not a defined target");
				}

				ProviderPtr CreateProviderFunction(const string& name)
				{
					throw FseException(name + " is not a defined provider");
				}

				ShaderPtr CreateShaderFunction(const string& name)
				{
					ShaderPtr shader;
					return shader;
				}
			}

			Linker::Linker()
				: providers(CreateProviderFunction)
				, targets(CreateTargetFunction)
				, shaders(CreateShaderFunction)
			{
			}

			Linker::~Linker()
			{
			}

			void Linker::addProvider(ProviderPtr prov)
			{
				providers.add(prov->getId(), prov);
			}

			void Linker::addTarget(TargetPtr targ)
			{
				targets.add(targ->getId(), targ);
			}

			TargetPtr Linker::getTarget(string name)
			{
				return targets.get(name);
			}

			ProviderPtr Linker::getProvider(const string& name)
			{
				return providers.get(name);
			}

			void Linker::storePipeline(PipelinePtr pl, TargetPtr target)
			{
				pl->add(target->getProvider());
			}

			void Linker::link()
			{
				BOOST_FOREACH(ProviderPtr p, providers.getData())
				{
					p->link(this);
				}

				BOOST_FOREACH(TargetPtr t, targets.getData())
				{
					t->link(this);
					if(t->getProvider() == 0)
					{
						throw FseException(t->toString() + " is missing a provider");
					}
				}

				BOOST_FOREACH(ProviderPtr p, providers.getData())
				{
					p->postlink(this);
				}
			}

			ShaderPtr Linker::getShaderOrNull(const string& id)
			{
				return shaders.get(id);
			}

			const string Linker::read(const string& path, ShaderPool* pool)
			{
				core::Ptree file;
				io::Read(path, &file);
				core::Ptree root = file.get_child("pipeline");
				const string t = root.get<string>("target");
				BOOST_FOREACH(const core::Ptree::value_type & shaderElement, root.get_child("shaders"))
				{
					const string& type = shaderElement.first;
					if(type == "shader")
					{
						const core::Ptree& data = shaderElement.second;
						const string& id = data.get<string>("id");
						ShaderPtr shader = pool->getFromSource(data.get_child("source"), core::Str() << path << ":" << id);
						shaders.add(id, shader);
					}
					else
					{
						throw "unknown element in shaders";
					}
				}
				BOOST_FOREACH(const core::Ptree::value_type & targetElement, root.get_child("targets"))
				{
					const string& type = targetElement.first;
					const core::Ptree& data = targetElement.second;
					TargetPtr target = Targets_Create(type, data);
					target->setId(data.get<string>("id"));
					addTarget(target);
				}
				BOOST_FOREACH(const core::Ptree::value_type & providerElement, root.get_child("providers"))
				{
					const string& type = providerElement.first;
					const core::Ptree& data = providerElement.second;
					ProviderPtr provider = Providers_Create(type, data, path);
					provider->setId(data.get<string>("id"));
					addProvider(provider);
				}

				return t;
			}
		}
	}
}
