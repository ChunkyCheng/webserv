#include "WebServer.hpp"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>

WebServer::WebServer(void)
{
	_epollfd = epoll_create(1);
	if (_epollfd == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		throw (EpollCreateException());
	}
	_servers.push_back(new Server(*this));
}

WebServer::~WebServer(void)
{
	for (unsigned int i = 0; i < _servers.size(); ++i)
		delete _servers[i];
	for (unsigned int i = 0; i < _clients.size(); ++i)
		delete _clients[i];
	if (_epollfd != -1)
		close(_epollfd);
}

int	WebServer::getEpollfd(void) const
{
	return (_epollfd);
}

void	WebServer::createClient(int fd, Server& server)
{
	_clients.push_back(new Client(fd, &server));
}

const char*	WebServer::EpollCreateException::what(void) const throw()
{
	return ("WebServer::EpollCreateExcpetion");
}
