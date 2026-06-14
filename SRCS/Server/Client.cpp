#include "Client.hpp"
#include "Server.hpp"
#include "Epoll.hpp"

Client::Client(int socket_fd, Server& server, Epoll& epoll)
	:_socket(*this, socket_fd), _server(server), _epoll(epoll),
	 _requestHandler(server, _request_buff, _response_buff, epoll, _socket)
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
	char	raw[4096];
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
		_requestHandler.processReqData();
		if (_requestHandler.checkRequestComplete())
		{
			_requestHandler.buildResponseData();
			if (!_requestHandler.isCgiPending())
				_epoll.modAddSendEvent(_socket);
		}
	}
}

void	Client::sendMessage(void)
{
	int	len;

	if (_response_buff.size() == 0)
		_requestHandler.continueBuildResponse();
	if (_requestHandler.checkResponseComplete() && _response_buff.size() == 0)
	{
		_requestHandler.reset();
		if (!_request_buff.empty())
		{
			_requestHandler.processReqData();
			if (_requestHandler.checkRequestComplete())
			{
				_requestHandler.buildResponseData();
				return ;
			}
		}
		_epoll.modRemoveSendEvent(_socket);
	}
	len = send(_socket.getFd(), _response_buff.c_str(), _response_buff.size(), 0);
	if (len == -1)
		std::cerr << strerror(errno) << std::endl;
	else
		_response_buff.erase(0, len);
}

void	Client::checkCgiTimeout(time_t now, int timeout_secs)
{
	if (_requestHandler.isCgiPending() && (now - _requestHandler.getCgiStartTime()) >= timeout_secs)
	{
		std::cerr << "CGI process timed out for client " << std::endl;
		_requestHandler.abortCgi();
	}
}