#include "WebServer.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"
#include <ctime>

static const int CGI_TIMEOUT_SECS = 10;

bool	WebServer::_runServer = false;

WebServer::WebServer(void)
{
	_initServer("webserv.conf");
}

WebServer::WebServer(const std::string config_path)
{
	_initServer(config_path);
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

void	WebServer::_initServer(const std::string config_path)
{
	ConfigParser	parser(config_path);
	unsigned int	i = 0;

	//parser.printTokens();
	if (parser.getTokens().size() == 1)
		return ;
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

void	WebServer::runServerLoop(void)
{
	WebServer::_runServer = true;
	if (_servers.size() == 0)
		return ;
	while (WebServer::_runServer)
	{
		_epoll.runEvents();
		_checkCgiTimeouts();
	}
}

void	WebServer::_checkCgiTimeouts(void)
{
	time_t now = time(NULL);
	for (size_t i = 0; i < _servers.size(); ++i)
		_servers[i]->checkCgiTimeouts(now, CGI_TIMEOUT_SECS);
}

void	WebServer::stopServerLoop(void)
{
	WebServer::_runServer = false;
}
