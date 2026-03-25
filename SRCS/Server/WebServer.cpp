#include "WebServer.hpp"
#include "Server.hpp"

bool	WebServer::_runServer = false;

WebServer::WebServer(void)
{
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
	while (WebServer::_runServer)
	{
		_epoll.runEvents();
	}
}

void	WebServer::stopServerLoop(void)
{
	WebServer::_runServer = false;
}
