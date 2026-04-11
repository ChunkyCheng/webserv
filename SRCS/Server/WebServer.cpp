#include "WebServer.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"

bool	WebServer::_runServer = false;

WebServer::WebServer(void)
{
	ConfigParser	parser("webserv.conf");
	unsigned int	i = 0;

	//parser.printTokens();
	parser.parseTokens();
	_servers = parser.createServers(*this);
	while (_servers.size())
	{
		std::cout << *_servers[i];
		++i;
		if (i == _servers.size())
			break;
		std::cout << "\n=====================================\n";
	}
	std::cout << std::endl;
}

WebServer::~WebServer(void)
{
	for (unsigned int i = 0; i < _servers.size(); ++i)
		delete _servers[i];
}

Epoll&	WebServer::getEpoll(void)
{
	return (_epoll);
}

void	WebServer::runServerLoop(void)
{
	WebServer::_runServer = true;
	if (_servers.size() == 0)
		return ;
	while (WebServer::_runServer)
	{
		_epoll.runEvents();
	}
}

void	WebServer::stopServerLoop(void)
{
	WebServer::_runServer = false;
}
