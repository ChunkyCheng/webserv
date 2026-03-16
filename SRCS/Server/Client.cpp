#include "Client.hpp"
#include <unistd.h>
#include <iostream>
#include "WebServer.hpp"
#include "Server.hpp"

Client::Client(int socket_fd, Server& server)
	:_socket(*this, socket_fd), _server(server)
{
}

Client::~Client(void)
{
}

ClientSocket& Client::getSocket(void)
{
	return (_socket);
}

void	Client::recvMessage(void)
{
	char	raw[1];
	int		len;

	len = recv(_socket.getFd(), raw, sizeof(raw), 0);
	if (len <= 0)
	{
		if (len == -1)
			std::cerr << strerror(errno) << std::endl;
		std::cout << _request_buff << std::endl;
		_server.deleteClient(_socket.getFd());
	}
	else
	{
		_request_buff += std::string(raw, len);
	}
}

void	Client::sendMessage(void)
{
	int	len;

	len = send(_socket.getFd(), _response_buff.c_str(), 1, 0);
	if (len == -1)
		std::cerr << strerror(errno) << std::endl;
	else
		_response_buff.erase(0, len);
}
