#include "Server.hpp"
#include <sys/socket.h>

Server::Server(void)
{
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
}

void	Server::open_listening_sockets(void)
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	int				gai_ret;

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	gai_ret = getaddrinfo((_port.substr(0, _port.find(':'))).c_str(),
						(_port.substr(_port.find(':') + 1)).c_str(),
						&hints, &res);
	if (gai_ret != 0)
	{
		std::cerr << gai_strerror(gai_ret)) << std::endl;
		return ;
	}
	_listening_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_listening_socket < 0)
	{
		return ;
	}
	bind(_listening_socket, res->ai_addr, res->ai_addrlen);
	freeaddinfo(res);
}
