#ifndef PWN_CORE_CONSOLE_ARGUMENTS_H
#define PWN_CORE_CONSOLE_ARGUMENTS_H

#include <boost/function.hpp>
#include <map>
#include <pwn/string.h>

namespace pwn
{
	namespace core
	{
		template <class Main>
		class ConsoleArguments
		{
		public:
			typedef boost::function<int (Main* main, ConsoleArguments* args)> Command;
			typedef std::map<pwn::string, Command> CommandMap;
			typedef boost::function<int (Main* main, ConsoleArguments* args, const pwn::string&)> CommandArg;
			typedef std::map<pwn::string, CommandArg> CommandArgMap;

			typedef std::map<pwn::string, std::vector<pwn::string> > AliasMap;
			typedef std::map<pwn::string, pwn::string> DescriptionMap;

			bool stopOnError;
			bool stop;
			pwn::string argv0;

			ConsoleArguments()
				: errors(0)
				, stopOnError(false)
				, stop(false)
			{
			}
			
			void setCommand(const std::vector<pwn::string>& names, Command arg, const pwn::string& description)
			{
				const pwn::string first = names[0];
				commands.push_back(first);

				BOOST_FOREACH(pwn::string name, names)
				{
					Assert( has(name) == false );
					cmd[name] = arg;
					descriptions[name] = description;
					aliases[name] = names;
				}
			}

			void setArgCommand(const std::vector<pwn::string>& names, CommandArg arg, const pwn::string& description)
			{
				const pwn::string first = names[0];
				commands.push_back(first);

				BOOST_FOREACH(pwn::string name, names)
				{
					Assert( has(name) == false );
					cmda[name] = arg;
					descriptions[name] = description;
					aliases[name] = names;
				}
			}

			void setMain(CommandArg arg)
			{
				cmdmain = arg;
			}

			bool has(const pwn::string& name) const
			{
				const bool missing = cmd.find(name) == cmd.end() && cmda.find(name)==cmda.end();
				return !missing;
			}

			void displayCommands()
			{
				BOOST_FOREACH(const pwn::string& n, commands)
				{
					cout << n << " ";
				}
				cout << std::endl;
			}

			const pwn::string displayAliases(const pwn::string& id) const
			{
				AliasMap::const_iterator it = aliases.find(id);
				if( it == aliases.end() ) return "";
				std::stringstream ss;
				BOOST_FOREACH(const pwn::string& n, it->second)
				{
					ss << n << " ";
				}
				return ss.str();
			}

			const pwn::string displayDescription(const pwn::string& id) const
			{
				DescriptionMap::const_iterator it = descriptions.find(id);
				if( it == descriptions.end() ) return "";
				return it->second;
			}

			const pwn::string displayUsage(const pwn::string& name) const
			{
				if( cmd.find(name) != cmd.end() )
				{
					return name;
				}
				else if( cmda.find(name) != cmda.end() )
				{
					return name + " ARG";
				}
				else
				{
					return "";
				}
			}

			void handle(int argc, char* argv[], Main* main)
			{
				argv0 = argv[0];

				for(int i=1; i<argc; ++i)
				{
					try
					{
						if( IsArgument(argv[i]) )
						{
							const std::string name = pwn::core::TrimLeft(argv[i], "-/");
							const std::string val = i+1<argc ? argv[i+1] : "";
							CommandMap::iterator cmdi = cmd.find(name);
							if( cmdi != cmd.end() )
							{
								errors += cmdi->second(main, this);
							}
							else
							{
								CommandArgMap::iterator cmdai = cmda.find(name);
								if( cmdai != cmda.end() )
								{
									errors += cmdai->second(main, this, val);
									++i;
								}
								else
								{
									std::cerr << "Unknown argument " << name << std::endl;
									++errors;
								}
							}
						}
						else
						{
							errors += cmdmain(main, this, argv[i]);
						}
					}
					catch(const std::string& err)
					{
						std::cerr << err << std::endl;
						++errors;
					}
					catch(const char* err)
					{
						std::cerr << err << std::endl;
						++errors;
					}
					catch(...)
					{
						std::cerr << "Unknown error" << std::endl;
						++errors;
					}

					if( stopOnError && errors > 0)
					{
						std::cerr << "Errors encounted, halting.." << std::endl;
						return;
					}

					if( stop )
					{
						return;
					}
				}
			}

			int getErrors() const
			{
				return errors;
			}
		private:
			static bool IsArgument(const std::string& str)
			{
				Assert(!str.empty());
				return str[0] == '-' || str[0] == '/';
			}

			CommandMap cmd;
			CommandArgMap cmda;
			CommandArg cmdmain;
			AliasMap aliases;
			DescriptionMap descriptions;
			std::vector<pwn::string> commands;
			int errors;
		};
	}
}

#endif
