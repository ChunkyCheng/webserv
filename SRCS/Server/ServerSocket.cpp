#include "ServerSocket.hpp"
#include <iostream>
#include <sys/epoll.h>
#include "WebServer.hpp"
#include "Server.hpp"

ServerSocket::ServerSocket(Server& server, std::string& socket_addr)
	:_server(server)
{
	std::string			ip;
	std::string			port;
	struct epoll_event	epoll_event;

	ip = socket_addr.substr(0, socket_addr.find(':'));
	port = socket_addr.substr(socket_addr.find(':') + 1);
	_open_socket(ip, port);
	epoll_event.events = EPOLLIN;
	epoll_event.data.ptr = this;
	epoll_ctl(_server.getWebServer().getEpollfd(), EPOLL_CTL_ADD, _fd, &epoll_event);
}

ServerSocket::~ServerSocket(void)
{
	if (_fd != -1)
		close(_fd);
}

void	ServerSocket::_open_socket(std::string& ip, std::string& port)
{
	struct addrinfo		hints;
	struct addrinfo*	res;
	int					gai_ret;

	hints = (struct addrinfo){AI_PASSIVE, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL, NULL};
	gai_ret = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
	if (gai_ret != 0)
	{
		std::cerr << gai_strerror(gai_ret) << std::endl;
		throw (GaiException());
	}
	_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_fd == -1)
	{
		freeaddrinfo(res);
		std::cerr << strerror(errno) << std::endl;
		throw (SocketCreateException());
	}
	if (bind(_fd, res->ai_addr, res->ai_addrlen) == -1)
	{
		freeaddrinfo(res);
		std::cerr << strerror(errno) << std::endl;
		throw (BindException());
	}
	freeaddrinfo(res);
}

void	ServerSocket::handleEvent(void)
{
	int	client_fd;

	client_fd = accept(_fd, NULL, NULL);
	if (client_fd == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		return ;
	}
	_server.getWebServer().createClient(client_fd, _server);
}

const char*	ServerSocket::GaiException::what(void) const throw()
{
	return ("ServerSocket::GaiException");
}

const char*	ServerSocket::SocketCreateException::what(void) const throw()
{
	return ("ServerSocket::SocketCreateException");
}

const char*	ServerSocket::BindException::what(void) const throw()
{
	return ("ServerSocket::BindException");
}
