#include "Server.hpp"
#include "WebServer.hpp"
#include "Client.hpp"
#include "Epoll.hpp"

Server::Server(WebServer& webserver, std::vector<Location> locations, Config& config)
	:_webserver(webserver),
	 _epoll(webserver.getEpoll()),
	 _socket_addr(config.listens),
	 _locations(locations)
{
	_open_listening_sockets();
}

Server::~Server(void)
{
	std::map<int, Client*>::iterator it;

	it = _clients.begin();
	while (it != _clients.end())
	{
		delete (*it).second;
		++it;
	}
	for (unsigned int i = 0; i < _listening_sockets.size(); ++i)
		delete _listening_sockets[i];
}

void	Server::_open_listening_sockets(void)
{
	for (unsigned int i = 0; i < _socket_addr.size(); ++i)
	{
		try
		{
			_listening_sockets.push_back(new ServerSocket(*this, _socket_addr[i]));
			_epoll.addSocketToPoll(*_listening_sockets.back());
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	if (_listening_sockets.size() == 0)
		throw (NoListeningSocketsException());
}

void	Server::createClient(int listening_fd)
{
	int		client_fd;
	Client	*newClient;

	client_fd = accept(listening_fd, NULL, NULL);
	if (client_fd == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		return ;
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	std::cout << "Client connected on socket " << listening_fd << std::endl;
	newClient = new Client(client_fd, *this, _epoll);
	_epoll.addSocketToPoll(newClient->getSocket());
	_clients[client_fd] = newClient;
}

void	Server::deleteClient(int client_fd)
{
	delete _clients[client_fd];
	_clients.erase(client_fd);
}

const char*	Server::NoListeningSocketsException::what(void) const throw()
{
	return ("Server::NoListeningSocketsException");
}
