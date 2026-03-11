#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ClientSocket.hpp"

class	Server;

class	Client
{
	public:
		Client(int socket_fd, Server* server);
		~Client(void);
	private:
		Client(void);
		Client(const Client& other);
		Client&	operator=(const Client& other);

	private:
		const ClientSocket	_socket;
		Server const*		_server;
};

#endif
