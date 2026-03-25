#include "Client.hpp"
#include "Server.hpp"
#include "Epoll.hpp"

Client::Client(int socket_fd, Server& server, Epoll& epoll)
	:_socket(*this, socket_fd), _server(server), _epoll(epoll),
	 _requestHandler(server, _request_buff, _response_buff)
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
		if (_requestHandler.checkRequestComplete())
			_epoll.modAddSendEvent(_socket);
	}
}

void	Client::sendMessage(void)
{
	int	len;

	if (_response_buff.size() == 0)
		_requestHandler.continueBuildResponse();	
	if (_requestHandler.checkResponseComplete() && _response_buff.size() == 0)
		_epoll.modRemoveSendEvent(_socket);
	len = send(_socket.getFd(), _response_buff.c_str(), 1, 0);
	if (len == -1)
		std::cerr << strerror(errno) << std::endl;
	else
		_response_buff.erase(0, len);
}
