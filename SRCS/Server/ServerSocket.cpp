#include "ServerSocket.hpp"
#include "Server.hpp"

ServerSocket::ServerSocket(Server& server, const std::string& socket_addr)
	:_server(server)
{
	std::string			ip;
	std::string			port;

	ip = socket_addr.substr(0, socket_addr.find(':'));
	port = socket_addr.substr(socket_addr.find(':') + 1);
	_open_socket(ip, port);	
}

ServerSocket::~ServerSocket(void)
{
	if (_fd != -1)
		close(_fd);
}

void	ServerSocket::_open_socket(std::string& ip, std::string& port)
{
	struct addrinfo		hints;
	struct addrinfo*	res;
	int					gai_ret;
	int					opt;

	hints = (struct addrinfo){AI_PASSIVE, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL, NULL};
	gai_ret = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);
	if (gai_ret != 0)
	{
		std::cerr << gai_strerror(gai_ret) << std::endl;
		throw (GaiException());
	}
	_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_fd == -1)
	{
		freeaddrinfo(res);
		std::cerr << strerror(errno) << std::endl;
		throw (SocketCreateException());
	}
	opt = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		freeaddrinfo(res);
		std::cerr << strerror(errno) << std::endl;
		throw (SocketOptionException());
	}
	if (bind(_fd, res->ai_addr, res->ai_addrlen) == -1)
	{
		freeaddrinfo(res);
		std::cerr << strerror(errno) << std::endl;
		throw (BindException());
	}
	freeaddrinfo(res);
	if (listen(_fd, SOMAXCONN) == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		throw (ListenException());
	}
	fcntl(_fd, F_SETFL, O_NONBLOCK);
}

int	ServerSocket::getFd(void) const
{
	return (_fd);
}

void	ServerSocket::handleEvent(int events)
{
	(void)events;
	_server.createClient(_fd);
}

const char*	ServerSocket::GaiException::what(void) const throw()
{
	return ("ServerSocket::GaiException");
}

const char*	ServerSocket::SocketCreateException::what(void) const throw()
{
	return ("ServerSocket::SocketCreateException");
}

const char*	ServerSocket::SocketOptionException::what(void) const throw()
{
	return ("ServerSocket::SocketOptionException");
}

const char*	ServerSocket::BindException::what(void) const throw()
{
	return ("ServerSocket::BindException");
}

const char*	ServerSocket::ListenException::what(void) const throw()
{
	return ("ServerSocket::ListenException");
}
