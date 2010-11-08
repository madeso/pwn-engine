#include <pwn/render/fse/Binder.h>
#include <pwn/render/fbo.h>
#include <pwn/render/fse/BufferReference.h>
#include <pwn/render/fse/exceptions.h>
#include <boost/foreach.hpp>
#include <pwn/render/fse/Linker.h>
#include <pwn/render/shaderpool.h>

namespace pwn
{
	namespace render
	{
		namespace fse
		{
			namespace
			{
				struct CreateFboFunc
				{
					CreateFboFunc(int w, int h)
						: width(w)
						, height(h)
					{
					}

					FboPtr operator()()
					{
						FboPtr fbo(new Fbo(width, height, false));
						return fbo;
					}

					int width; int height;
				};

				template<typename T>
				struct GetFromPoolFunc
				{
					GetFromPoolFunc(core::Pool<T>* p)
						: pool(p)
					{
					}

					core::Pool<T>* pool;

					T operator()(const string&)
					{
						return pool->get();
					}
				};
			}
			
			FboPool::FboPool(int width, int height)
				: pool( CreateFboFunc(width, height) )
				, map( GetFromPoolFunc<FboPtr>(&pool) )
			{
			}

			FboPool::~FboPool()
			{
			}
			
			FboPtr FboPool::create(const string& name)
			{
				return map.get(name);
			}
			
			void FboPool::release(FboPtr fbo)
			{
				pool.release(fbo);
			}

			namespace 
			{
				class ShaderLoader
				{
				public:
					ShaderLoader(ShaderPool* s)
						: pool(s)
					{}

					ShaderPtr operator()(const string& file)
					{
						return pool->getFromFile(file);
					}
				private:
					ShaderPool* pool;
				};
			}
			
			Binder::Binder(ShaderPool* pool)
				: shaders( ShaderLoader(pool) )
			{
			}

			Binder::~Binder()
			{
			}
			
			ShaderPtr Binder::getShader(const string& shadername)
			{
				return shaders.get(shadername);
			}
			
			ShaderPtr Binder::getShaderOrNull(const string& shadername)
			{
				if (shadername.empty()) return ShaderPtr();
				else return shaders.get(shadername);
			}
			
			void Binder::reference(BufferReferencePtr br)
			{
				if (references.find(br) != references.end())
				{
					throw FseException(br->getName() + " already addd to " + toString());
				}
				references.insert(br);
			}
			
			namespace
			{
				class StringCounter
				{
				public:
					typedef std::map<string, unsigned int> Map;

					void add(const string& s)
					{
						map[s] = countsOf(s)+1;
					}

					unsigned int countsOf(const string& s) const
					{
						const Map::const_iterator f = map.find(s);
						return f!=map.end()
							? f->second
							: 0;
					}
				private:
					Map map;
				};
			}
			
			void Binder::createBuffers()
			{
				StringCounter sc;
				BOOST_FOREACH(BufferReferencePtr b, references)
				{
					sc.add(b->getName());
				}

				StringCounter usages;
				BOOST_FOREACH(BufferReferencePtr b, references)
				{
					FboPtr buff = allocate(b->getName());
					b->setBuffer(buff);
					usages.add(b->getName());
					if (usages.countsOf(b->getName()) == sc.countsOf(b->getName()))
					{
						release(buff);
					}
				}
			}
			
			void Binder::associate(const string& p, Size size)
			{
				associations[p] = size;
			}
			
			FboPtr Binder::allocate(const string& name)
			{
				return getCreator(name)->create(name);
			}
			FboCreatorPtr Binder::getCreator(const string& name)
			{
				return getCreator(sizeOf(name));
			}
			
			FboCreatorPtr Binder::getCreator(Size size)
			{
				CreatorMap::iterator result = creators.find(size);
				if (result == creators.end())
				{
					FboCreatorPtr c( new FboPool(size.width, size.height) );
					creators[size] = c;
					return c;
				}
				else
				{
					return result->second;
				}
			}
			
			void Binder::release(FboPtr buff)
			{
				getCreator(
					Size(buff->getWidth(), buff->getHeight())
					)->release(buff);
			}

			string Binder::toString() const
			{
				return "";
			}
			
			Size Binder::sizeOf(const string& name) const
			{
				AssociationMap::const_iterator r = associations.find(name);
				
				if (r == associations.end())
				{
					throw FseException(name + " is missing a defined size");
				}
				
				else return r->second;
			}
		}
	}
}
