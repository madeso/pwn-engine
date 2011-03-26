#include "An8.hpp"

#include <vector>
#include <fstream>

namespace pwn
{
	namespace convert
	{
		namespace an8
		{
			struct Term
			{
				enum Type
				{
					Number, String, Ident, Begin, End
				};

				Term(Type t)
					: type(t)
				{
				}

				Term(Type t, const pwn::string& val)
					: type(t)
					, value(val)
				{
				}

				Type type;
				pwn::string value;
			};

			class Lexer
			{
			public:
				enum State
				{
					sNone, sIdent, sString, sNumber, sNumberWithoutDot
				};

				void on(char c)
				{
					switch(state)
					{
					case sNone:
						if( IsWhitespace(c) ) return;
						if( IsLetter(c) )
						{
							memory += c;
							state = sIdent;
						}
						else if( IsNumber(c) )
						{
							memory += c;
							state = sNumber;
						}
						else if( c == '{' )
						{
							terms.push_back(Term(Term::Begin));
						}
						else if( c == '}' )
						{
							terms.push_back(Term(Term::End));
						}
						else if ( c == '\"' )
						{
							state = sString;
						}
						else 
						{
							throw "not a known sign";
						}
						break;
					case sString:
						if( ignoreChar )
						{
							switch(c)
							{
							case 't':
								memory += '\t';
								break;
							case 'n':
								memory += '\n';
								break;
							case '\"':
								memory += '\"';
								break;
							case '\\':
								memory += '\\';
								break;
							default:
								throw "unknown escape";
								break;
							}
						}
						else
						{
							if( c == '\\' )
							{
								ignoreChar = true;
							}
							else if ( c == '\"' )
							{
								terms.push_back(Term(Term::String, memory));
								memory.clear();
								state = sNone;
							}
							else
							{
								memory += c;
							}
						}
						break;
					case sNumber:
						if( IsNumber(c) )
						{
							memory += c;
						}
						else if( c == '.' )
						{
							memory += '.';
							state = sNumberWithoutDot;
						}
						else
						{
							terms.push_back(Term(Term::Number, memory));
							memory.clear();
							state = sNone;
							on(c);
						}
						break;
					case sNumberWithoutDot:
						if( IsNumber(c) )
						{
							memory += c;
						}
						else
						{
							terms.push_back(Term(Term::Number, memory));
							memory.clear();
							state = sNone;
							on(c);
						}
						break;
					case sIdent:
						if( IsLetter(c) || IsNumber(c) )
						{
							memory += c;
						}
						else
						{
							terms.push_back(Term(Term::Ident, memory));
							memory.clear();
							state = sNone;
							on(c);
						}
						break;
					default:
						throw "unknown lexer state";
					}
				}

				void complete()
				{
					if( state == sNone ) return;
					if( state == sString ) throw "invalid string syntax";
					if( state == sNumber || state == sNumberWithoutDot )
					{
						terms.push_back(Term(Term::Number, memory));
						memory.clear();
						state = sNone;
					}
					if( state == sIdent )
					{
						terms.push_back(Term(Term::Ident, memory));
						memory.clear();
						state = sNone;
					}
					throw "unknown ending state";
				}

				std::vector<Term> terms;
			private:
				bool ignoreChar;
				State state;
				pwn::string memory;

				bool IsWhitespace(char c)
				{
					const static pwn::string s(" \n\t");
					return s.find_first_of(c) != -1;
				}
				bool IsLetter(char c)
				{
					const static pwn::string s("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
					return s.find_first_of(c) != -1;
				}
				bool IsNumber(char c)
				{
					const static pwn::string s("0123456789");
					return s.find_first_of(c) != -1;
				}
			};

			void read(pwn::mesh::Builder* builder, const pwn::string& file)
			{
				std::ifstream f(file.c_str());
				if( f.good() == false ) throw "failed to open file";
				char c;
				Lexer lex;
				while( f >> c)
				{
					lex.on(c);
				}
				lex.complete();
			}
		}
	}
}