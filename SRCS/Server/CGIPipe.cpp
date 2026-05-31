#include "CGIPipe.hpp"
#include "Client.hpp"
#include <sys/epoll.h>

CGIPipe::CGIPipe(Client& client, CGIHandler& cgiHandler)
	: _client(client), _cgiHandler(cgiHandler), _fd(cgiHandler.getStdoutFd())
{
}

CGIPipe::~CGIPipe(void)
{
}

int CGIPipe::getFd(void) const
{
	return (_fd);
}

void CGIPipe::handleEvent(int events)
{
	if (events & EPOLLIN)
	{
		_cgiHandler.collectOutputPartial();
		if (_cgiHandler.getStdoutFd() == -1)
		{
			_client.onCgiComplete(*this);
			return;
		}
	}
	if (events & (EPOLLHUP | EPOLLERR))
	{
		_cgiHandler.collectOutputPartial();
		_client.onCgiComplete(*this);
	}
}
