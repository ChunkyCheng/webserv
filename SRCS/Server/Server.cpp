#include "Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

Server::Server(void)
	:_port("0.0.0.0:8080")
{
	_open_listening_sockets();
}

Server::Server(const Server& other)
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
	struct addrinfo	hints;
	struct addrinfo	*res;
	int				gai_ret;

	hints = (struct addrinfo){AI_PASSIVE, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL, NULL};
	gai_ret = getaddrinfo((_port.substr(0, _port.find(':'))).c_str(),
						(_port.substr(_port.find(':') + 1)).c_str(),
						&hints, &res);
	if (gai_ret != 0)
	{
		std::cerr << gai_strerror(gai_ret) << std::endl;
		return ;
	}
	_listening_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_listening_socket < 0)
	{
		return ;
	}
	bind(_listening_socket, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
}
