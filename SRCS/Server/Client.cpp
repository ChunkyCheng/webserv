#include "Client.hpp"
#include <unistd.h>

Client::Client(void)
	:_socket(-1)
{
}

Client::Client(const Client& other)
	:_socket(-1)
{
	(void)other;
}

Client&	Client::operator=(const Client& other)
{
	if (this != &other)
	{
	}
	return (*this);
}

Client::~Client(void)
{
	if (_socket != -1)
		close(_socket);
}
