#include "Client.hpp"
#include "CGIPipe.hpp"
#include "Server.hpp"
#include "Epoll.hpp"
#include <ctime>

Client::Client(int socket_fd, Server& server, Epoll& epoll)
	:_socket(*this, socket_fd), _server(server), _epoll(epoll),
	 _requestHandler(server, _request_buff, _response_buff), _cgiPipe(NULL)
{
}

Client::~Client(void)
{
	if (_cgiPipe)
	{
		_epoll.removeSocketFromPoll(*_cgiPipe);
		delete _cgiPipe;
	}
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
			if (_requestHandler.hasPendingCgi())
			{
				_cgiPipe = new CGIPipe(*this, *_requestHandler.getCgiHandler());
				_epoll.addSocketToPoll(*_cgiPipe);
			}
			else
			{
				_epoll.modAddSendEvent(_socket);
			}
		}
	}
}

void	Client::closeCgiPipe(void)
{
	_epoll.removeSocketFromPoll(*_cgiPipe);
	delete _cgiPipe;
	_cgiPipe = NULL;
	_epoll.modAddSendEvent(_socket);
}

void	Client::onCgiComplete(CGIPipe& pipe)
{
	(void)pipe;
	_requestHandler.finalizeCgiResponse();
	closeCgiPipe();
}

void	Client::checkCgiTimeout(void)
{
	const int	CGI_TIMEOUT = 5;

	if (!_cgiPipe)
		return;
	CGIHandler*	handler = _requestHandler.getCgiHandler();
	if (!handler)
		return;
	if (time(NULL) - handler->getStartTime() > CGI_TIMEOUT)
	{
		_requestHandler.abortCgiWithError(INTERNAL_SERVER_ERROR);
		closeCgiPipe();
	}
}

void	Client::sendMessage(void)
{
	int	len;

	if (_response_buff.size() == 0)
		_requestHandler.continueBuildResponse();
	if (_requestHandler.checkResponseComplete() && _response_buff.size() == 0)
	{
		if (_requestHandler.getShouldCloseConnection())
		{
			_server.deleteClient(_socket.getFd());
			return ;
		}
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
	{
		std::cerr << strerror(errno) << std::endl;
		// In case of server running infinite loop on broken pipe/bad client/bad fd, delete client, can preserve server resources too
		// if (errno != EAGAIN && errno != EWOULDBLOCK)
		// {
		// 	std::cerr << strerror(errno) << std::endl;
		// 	_server.deleteClient(_socket.getFd());
		// }
	}
	else
		_response_buff.erase(0, len);
}
