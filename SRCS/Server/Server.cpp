#include "Server.hpp"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

Server::Server(int epollfd)
	:_epollfd(epollfd),_port("0.0.0.0:8080")
{
	_open_listening_sockets();
	_add_to_epoll();
}

Server::Server(void)
	:_epollfd(-1)
{
}

Server::Server(const Server& other)
	:_epollfd(-1)
{
	(void)other;
}

Server&	Server::operator=(const Server& other)
{
	if (this != &other)
	{
	}
	return (*this);
}

Server::~Server(void)
{
	close(_listening_socket);
}

void	Server::_open_listening_sockets(void)
{
	struct addrinfo		hints;
	struct addrinfo*	res;
	int					gai_ret;

	hints = (struct addrinfo){AI_PASSIVE, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL, NULL};
	gai_ret = getaddrinfo((_port.substr(0, _port.find(':'))).c_str(),
						(_port.substr(_port.find(':') + 1)).c_str(),
						&hints, &res);
	if (gai_ret != 0)
	{
		std::cerr << gai_strerror(gai_ret) << std::endl;
		throw (GaiException());
	}
	_listening_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_listening_socket == -1)
	{
		freeaddrinfo(res);
		std::cerr << strerror(errno) << std::endl;
		throw (SocketCreateException());
	}
	if (bind(_listening_socket, res->ai_addr, res->ai_addrlen) == -1)
	{
		freeaddrinfo(res);
		std::cerr << strerror(errno) << std::endl;
		throw (BindException());
	}
	freeaddrinfo(res);
}

void	Server::_add_to_epoll(void)
{
	struct epoll_event	epoll_event;

	epoll_event.events = EPOLLIN;
	epoll_event.data.ptr = this;
	epoll_ctl(_epollfd, EPOLL_CTL_ADD, _listening_socket, &epoll_event);
}

const char*	Server::GaiException::what(void) const throw()
{
	return ("Server::GaiException");
}

const char*	Server::SocketCreateException::what(void) const throw()
{
	return ("Server::SocketCreateException");
}

const char*	Server::BindException::what(void) const throw()
{
	return ("Server::BindException");
}
