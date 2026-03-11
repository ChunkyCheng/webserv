#include "Client.hpp"
#include <unistd.h>

Client::Client(int socket_fd, Server* server)
	:_socket(*this, socket_fd), _server(server)
{
}

Client::~Client(void)
{
}
