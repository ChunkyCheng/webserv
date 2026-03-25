#include "ClientSocket.hpp"
#include "Client.hpp"

ClientSocket::ClientSocket(Client& client, int fd)
	:_client(client), _fd(fd)
{
}

ClientSocket::~ClientSocket(void)
{
	if (_fd != -1)
		close(_fd);
}

int	ClientSocket::getFd(void) const
{
	return (_fd);
}

void	ClientSocket::handleEvent(int events)
{
	if (events & EPOLLIN)
		_client.recvMessage();
	if (events & EPOLLOUT)
		_client.sendMessage();
}
