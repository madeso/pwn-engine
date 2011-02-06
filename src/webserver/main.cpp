#include "mongoose.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>

class Request
{
public:
	Request(mg_connection *conn)
		: conn(conn)
	{
	}

	const std::string getVariable(const std::string& name, const std::string& def) const
	{
		char* val = mg_get_var(conn, name.c_str());
		if( val == 0 ) return def;
		const std::string res = val;
		if( val ) free(val);
		return res;
	}

private:
	mg_connection* conn;
};

class Page
{
public:
	Page()
	{
	}
	virtual ~Page()
	{
	}

	virtual void response(const Request& req, std::stringstream& stream) = 0;
};

static void bar(struct mg_connection *conn, const struct mg_requst_info *ri, void *data)
{
	Page* page = reinterpret_cast<Page*>(data);

	mg_printf(conn, "%s",
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Connection: close\r\n\r\n");

	std::stringstream ss;
	page->response(Request(conn), ss);

	// Output HTML page
	mg_printf(conn,
		"%s", ss.str().c_str());
}

class Server
{
public:
	Server(const std::string& port)
		: ctx(mg_start())
	{
		mg_set_option(ctx, "root", "/var/www");
		mg_set_option(ctx, "ports", port.c_str());
	}
	~Server()
	{
	}

	void addPage(const std::string& uri, Page* page)
	{
		mg_bind_to_uri(ctx, uri.c_str(), reinterpret_cast<mg_callback_t>(&bar), page);
	}

	mg_context* ctx;
};

class SamplePage : public Page
{
public:
	void response(const Request& req, std::stringstream& stream)
	{
		stream << "<html><body>"
			<< "Hello " << req.getVariable("name", "unknown person") << "! I hope you feel great"
			<< "<br>[<a href=\"counter1\">counter1</a>] "
			<< "[<a href=\"counter2\">counter2</a>]"
			<< "</body></html>";
	}
};

class CounterPage : public Page
{
public:
	CounterPage() : count(0) {}
	void response(const Request& req, std::stringstream& stream)
	{
		stream << "<html><body>";
		stream << "Hello, counter is <b>" << count << "</b>! I hope this works";
		if( count > 2 ) { stream << "<br/>Yes it does!"; }
		if( count <3 ) stream << "<br>[<a href=\"hello?name=my%20creator\">Bakk</a>]";
		stream << "</body></html>";
		++count;
	}
private:
	int count;
};

typedef boost::shared_ptr<Page> PagePtr;

void add(Server& server, std::vector<PagePtr>& container, const std::string& uri, Page* page)
{
	PagePtr p(page);
	server.addPage(uri, page);
	container.push_back(p);
}

void main()
{
	Server server("12345");
	std::vector<PagePtr> container;
	add(server, container, "/hello", new SamplePage() );
	add(server, container, "/counter1", new CounterPage() );
	add(server, container, "/counter2", new CounterPage() );
	// Now Mongoose is up, running and configured.
	// Serve until somebody terminates us
	std::cin.get();
}
