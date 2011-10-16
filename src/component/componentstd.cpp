#include <pwn/component/componentstd.h>
#include <pwn/component/component.h>
#include <pwn/component/eventargs.h>

#include <functional>
#include <boost/bind.hpp>

#define BEGIN_EVENT_TABLE() namespace { template<class X> void RegisterCallbacks(X* x) {
#define REGISTER_CALLBACK(e,f) x->addCallback(::pwn::component::EventArgs::Type().toEnum(#e),boost::bind(std::mem_fun(&X::f), x, _1))
#define END_EVENT_TABLE() } }
#define DECLARE_CALLBACK() void registerCallbacks()
#define IMPLEMENT_CALLBACK(X) void X::registerCallbacks() { RegisterCallbacks(this); }

BEGIN_EVENT_TABLE()
	REGISTER_CALLBACK(Update, onUpdate);
END_EVENT_TABLE()

namespace pwn
{
	namespace component
	{
		class ComponentTimeout
			: public Component
		{
		public:
			void onUpdate(const EventArgs& a)
			{
			}

			DECLARE_CALLBACK();

			static boost::shared_ptr<Component> Create()
			{
				boost::shared_ptr<Component> c(new ComponentTimeout());
				return c;
			}
		private:
		};

		IMPLEMENT_CALLBACK(ComponentTimeout)
	}
}
