#ifndef PWN_RENDER_FSE_MAP
#define PWN_RENDER_FSE_MAP

#include <map>
#include <vector>
#include <boost/function.hpp>
#include <pwn/string.h>

namespace pwn
{
	namespace core
	{
		template<typename T>
		class Map
		{
		private:
			typedef std::map<string, T> M;
		public:
			typedef boost::function<T (const string&)> Generator;
		public:
			Map(Generator g)
				: generator(g)
			{
			}

			T get(const string& var)
			{
				typename M::iterator r = m.find(var);
				if( r != m.end() ) return r->second;

				T t = generator(var);
				add(var, t);
				return t;
			}

			void add(const string& name, T t)
			{
				m.insert(M::value_type(name, t));
				v.push_back(t);
			}

			const std::vector<T> getData() const
			{
				return v;
			}
		private:
			Generator generator;
			M m;
			std::vector<T> v;
		};
	}
}

#endif
