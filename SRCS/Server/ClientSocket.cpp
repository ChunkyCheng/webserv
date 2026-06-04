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
	if (events & EPOLLOUT)
		_client.sendMessage();
	else if (events & EPOLLIN) // changed from if to else if to prevent reading into client that are already gone in the first place
		_client.recvMessage();
}

// handleEvent might need else if to prevent recvMessage and sendMessage from being called in the same event loop iteration, but for now we can leave it as is since the client will only be in EPOLLIN or EPOLLOUT state at any given time, not both.