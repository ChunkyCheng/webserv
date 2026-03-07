#include "WebServer.hpp"
#include <iostream>
#include <cerrno>
#include <cstring>
#include "unistd.h"

WebServer::WebServer(void)
{
	_epollfd = epoll_create(1);
	if (_epollfd == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		throw (EpollCreateException());
	}
	_servers.push_back(new Server(_epollfd));
}

WebServer::WebServer(const WebServer& other)
{
	(void)other;
}

WebServer&	WebServer::operator=(const WebServer& other)
{
	if (this != &other)
	{
	}
	return (*this);
}

WebServer::~WebServer(void)
{
	for (unsigned int i = 0; i < _servers.size(); ++i)
		delete _servers[i];
	if (_epollfd != -1)
		close(_epollfd);
}

const char*	WebServer::EpollCreateException::what(void) const throw()
{
	return ("WebServer::EpollCreateExcpetion");
}
