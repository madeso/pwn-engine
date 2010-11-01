#ifndef PWN_RENDER_FSE_TYPES
#define PWN_RENDER_FSE_TYPES

#include <boost/shared_ptr.hpp>
#include <pwn/string.h>
namespace pwn
{
	namespace render
	{
		// temporary
		class CompiledCamera;
		class World3;
		
		class Fbo;		typedef boost::shared_ptr<Fbo> FboPtr;

		namespace fse
		{
			class FboPool;		typedef boost::shared_ptr<FboPool> FboCreatorPtr;
			class BufferReference;	typedef boost::shared_ptr<BufferReference> BufferReferencePtr;
			class Provider;			typedef boost::shared_ptr<Provider> ProviderPtr;
			class Target;			typedef boost::shared_ptr<Target> TargetPtr;
			class Linker;			typedef boost::shared_ptr<Linker> LinkerPtr;
			class Binder;
			class Command;			typedef boost::shared_ptr<Command> CommandPtr;
			class Pipeline;			typedef boost::shared_ptr<Pipeline> PipelinePtr;
		}
	}
}

#endif
