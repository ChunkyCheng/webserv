#include "WebServer.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"

bool	WebServer::_runServer = false;

WebServer::WebServer(void)
{
	ConfigParser	parser("webserv.conf");

	parser.printTokens();
	if (parser.parseTokens() == false)
		throw (std::exception());
	_servers.push_back(new Server(*this, _epoll));
}

WebServer::~WebServer(void)
{
	for (unsigned int i = 0; i < _servers.size(); ++i)
		delete _servers[i];
}

void	WebServer::runServerLoop(void)
{
	WebServer::_runServer = true;
	if (_servers.size() == 0)
		return ;
	while (WebServer::_runServer)
	{
		_epoll.runEvents();
	}
}

void	WebServer::stopServerLoop(void)
{
	WebServer::_runServer = false;
}
