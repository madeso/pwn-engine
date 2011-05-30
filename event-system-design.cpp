DECLARE_EVENT_TABLE();

BEGIN_EVENT_TABLE()
	ON_EVENT(MyEvent, MyEventCallback)
END_EVENT_TABLE()

void MyEventCallback(const EventArgs& args)

static const seVak MyEvent = seType.get("event")
static const seVal kEventArg = MyEvent.get("arg");

real r = args.get<kRealType>(kEventArg);
real* o = args.optional<kRealArgType>(kEventArg);

void MyEventCallback(const EventArgs& args)
{ MyVentCallback(MyEventArgs(args)); }

struct MyEventArgs
{
	real r;
	
	MyEventArgs(const EventArgs& args);
	static bool isValid(const seVal& eventType);
};

void MyEventCallback(const MyEventArgs& args)
{
	// user implemented
}