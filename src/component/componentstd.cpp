#include <pwn/component/componentstd.h>
#include <pwn/component/component.h>
#include <pwn/component/eventargs.h>
#include <pwn/component/property.h>
#include <pwn/component/componentdef.h>
#include <pwn/component/object.h>

namespace pwn
{
	namespace component
	{
		class ComponentTimeout
			: public Component
		{
		public:
			ComponentTimeout(const ComponentArgs& args)
				: time(locals.refReal())
				, ev(args.get("event")->getEvent())
			{
				time = args.get("time")->getReal();
			}

			void onUpdate(const EventArgs& a)
			{
				const real delta = a[0].getReal();
				time -= delta;
				if(time < delta)
				{
					markForRemoval();
					sendObjectEvent(ev, EventArgs());
				}
			}

			DECLARE_CALLBACK();

			static boost::shared_ptr<Component> Create(const PropertyMap&, const ComponentArgs& args)
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

		//-----------------------------------------------------------------------------------------------

		class ComponentReboundingHealth
			: public Component
		{
		public:
			ComponentReboundingHealth(const PropertyMap& props, const ComponentArgs& args)
				: health(props.get("health")->refReal())
				, currentPauseTime(locals.refReal())
				, regenPower(args.get("regen")->getReal())
				, waitTime(args.get("wait")->getReal())
			{
			}

			void onUpdate(const EventArgs& args)
			{
				const real delta = args[0].getReal();
				if(currentPauseTime <= 0)
				{
					health += regenPower * delta;
				}
				else
				{
					currentPauseTime -= delta;
				}
			}

			void onDamage(const EventArgs& args)
			{
				currentPauseTime = waitTime;
			}

			static boost::shared_ptr<Component> Create(const PropertyMap& props, const ComponentArgs& args)
			{
				boost::shared_ptr<Component> c(new ComponentReboundingHealth(props, args));
				return c;
			}

			DECLARE_CALLBACK();
		private:
			real& health;
			real& currentPauseTime;
			const real regenPower;
			const real waitTime;
		};

		BEGIN_EVENT_TABLE(ComponentReboundingHealth)
			REGISTER_CALLBACK(Update, onUpdate);
			REGISTER_CALLBACK(Damage, onDamage);
		END_EVENT_TABLE()

		//-----------------------------------------------------------------------------------------------

		ComponentCreator::ComponentCreator()
		{
		}

		ComponentCreator::~ComponentCreator()
		{
		}

		boost::shared_ptr<Component> ComponentCreator::create(const ID& name, const PropertyMap& props, const ComponentArgs& args) const
		{
			Functions::const_iterator r = functions.find(name);
			if(r == functions.end())
			{
				throw "unknown component";
			}
			return r->second(props, args);
		}

		void ComponentCreator::add(const ID& name, CreateFunction func)
		{
			functions.insert(Functions::value_type(name, func));
		}

		//-----------------------------------------------------------------------------------------------

		void AddStandardComponents(ComponentCreator* cc)
		{
			cc->add("rebounding-health", ComponentReboundingHealth::Create);
			cc->add("timeout", ComponentTimeout::Create);
		}
	}
}
