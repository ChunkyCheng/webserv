#include "Server.hpp"
#include "WebServer.hpp"
#include <iostream>

Server::Server(WebServer& webserver)
	:_webserver(webserver)
{
	_socket_addr.push_back("0.0.0.0:8080");
	_open_listening_sockets();
}

Server::~Server(void)
{
	for (unsigned int i = 0; i < _listening_sockets.size(); ++i)
		delete _listening_sockets[i];
}

WebServer&	Server::getWebServer(void)
{
	return(_webserver);
}

void	Server::_open_listening_sockets(void)
{
	for (unsigned int i = 0; i < _socket_addr.size(); ++i)
	{
		try
		{
			_listening_sockets.push_back(new ServerSocket(*this, _socket_addr[i]));
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}
