#include <pwn/component/componentstd.h>
#include <pwn/component/component.h>
#include <pwn/component/eventargs.h>
#include <pwn/component/property.h>
#include <pwn/component/componentdef.h>

namespace pwn
{
	namespace component
	{
		class ComponentTimeout
			: public Component
		{
		public:
			ComponentTimeout(const ComponentArgs& args)
				: time(locals.getReal())
				, ev(args.get("event")->getEvent())
			{
				time = args.get("time")->getReal();
			}

			void onUpdate(const EventArgs& a)
			{
				const real delta = a[0].getReal();
				time -= delta;
				if( time < delta )
				{
					markForRemoval();
					sendObjectEvent(ev, EventArgs());
				}
			}

			DECLARE_CALLBACK();

			static boost::shared_ptr<Component> Create(const ComponentArgs& args)
			{
				boost::shared_ptr<Component> c(new ComponentTimeout(args));
				return c;
			}
		private:
			core::EnumValue ev;
			real& time;
		};

		BEGIN_EVENT_TABLE(ComponentTimeout)
			REGISTER_CALLBACK(Update, onUpdate);
		END_EVENT_TABLE()

		IMPLEMENT_CALLBACK(ComponentTimeout)
	}
}
