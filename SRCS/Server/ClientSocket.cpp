#include "ClientSocket.hpp"

ClientSocket::ClientSocket(Client& client, int fd)
	:_client(client), _fd(fd)
{
}

ClientSocket::~ClientSocket(void)
{
	if (_fd != -1)
		close(_fd);
}

void	ClientSocket::handleEvent(void)
{
}
