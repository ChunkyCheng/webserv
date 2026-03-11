#include "WebServer.hpp"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>

bool	WebServer::_runServer = false;

WebServer::WebServer(void)
{
	_servers.push_back(new Server(*this));
	_epoll.addServerToPoll(*_servers[0]);
}

WebServer::~WebServer(void)
{
	for (unsigned int i = 0; i < _servers.size(); ++i)
		delete _servers[i];
	for (unsigned int i = 0; i < _clients.size(); ++i)
		delete _clients[i];
}

void	WebServer::createClient(int fd, Server& server)
{
	Client*	newClient = new Client(fd, &server);

	_clients.push_back(newClient);
	_epoll.addClientToPoll(newClient->getSocket());
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
