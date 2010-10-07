﻿#include <pwn/render/fse/Provider.h>
#include <pwn/render/fse/Linker.h>
#include <pwn/render/fse/exceptions.h>
#include <pwn/render/fse/Target.h>
#include <pwn/render/fse/Command.h>
#include <boost/foreach.hpp>
#include <pwn/render/fse/BufferReference.h>
#include <pwn/render/fse/Command_Commands.h>
#include <pwn/core/StringSeperator.h>
#include <pwn/core/Str.h>
#include <pwn/render/fse/Binder.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			Link::Link(const string& aname)
				: name(aname)
			{
			}

			Link::~Link()
			{
			}
			
			void Link::provide(RenderArgs ra)
			{
				prov->doProvide(ra);
			}
			
			void Link::sortout(LinkerPtr usr)
			{
				prov = usr->getProvider(name);
			}
			
			void Provider::setId(const string& aid)
			{
				if (id.empty() ) id = aid;
				else throw FseException("Unable to change id from " + id + " to " + aid);
			}
			
			const string Provider::getId() const
			{
				return id;
			}
			
			void Provider::provide(const RenderArgs& ra)
			{
				if (autocallCommands)
				{
					callCommands();
				}

				target->apply(*this, ra);
			}
			
			std::vector<Provider*>& Provider::getProviders()
			{
				return providers;
			}
			
			void Provider::link(Linker* linker)
			{
				if (false == targetname.empty())
				{
					try
					{
						target = linker->getTarget(targetname);
					}
					catch(FseException& e)
					{
						e.addMessage(core::Str() << "From provider: " << toString());
						throw;
					}
					target->setProvider(this);
				}

				BOOST_FOREACH(CommandPtr c, commands)
				{
					c->link(linker);
				}

				doLink(linker);
			}
			
			void Provider::bind(Binder* bd)
			{
				const string file = shaderdata.data();
				if( file.empty() )
				{
					boost::optional<const core::Ptree&> source = shaderdata.get_child_optional("source");

					if( source )
					{
						shader = Shader::Create(source.get());
					}
				}
				else
				{
					shader = bd->getShader(file);
				}
				doBind(bd);

				BOOST_FOREACH(CommandPtr c, commands)
				{
					c->bind(bd);
				}
			}
			
			TargetPtr Provider::getTarget()
			{
				return target;
			}
			
			BufferReferencePtr Provider::createBuffer(const string& name)
			{
				return BufferReferencePtr(new BufferReference(name));
			}
			
			void Provider::postlink(Linker* linker)
			{
				BOOST_FOREACH(CommandPtr c, commands)
				{
					c->link(linker);

					BOOST_FOREACH(Provider* p, c->getDependencies())
					{
						providers.push_back(p);
					}
				}
			}

			ShaderPtr Provider::getShader()
			{
				return shader;
			}
			
			void Provider::denyAutocallOfCommands()
			{
				autocallCommands = false;
			}
			
			void Provider::callCommands()
			{
				BOOST_FOREACH(CommandPtr c, commands)
				{
					c->apply();
				}
			}
			
			Provider::Provider(const core::Ptree& el)
				: autocallCommands(true)
				, shaderdata(el.get_child("shader"))
				, targetname(el.get<string>("target"))
			{
				// foreach (const core::Ptree& e in Xml.Elements(el))
				BOOST_FOREACH (const core::Ptree::value_type& e, el.get_child("commands"))
				{
					const string& type = e.first;
					const core::Ptree& data = e.second;
					
					commands.push_back(Commands_Create(type, data, this));
				}
			}

			Provider::~Provider()
			{
			}
			
			string Provider::toString() const
			{
				return core::Str() << getId() << "(" << targetname <<  "): <" << core::StringSeperator().english().iterate(commands).toString() << ">";
			}
		}
	}
}