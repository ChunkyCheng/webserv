#include "Client.hpp"

Client::Client(void)
{
}

Client::Client(const Client& other)
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
