#ifndef SERVER_HPP
# define SERVER_HPP

# include "socket.h"
# include "ServerSocket.hpp"

class	WebServer;
class	Epoll;

class	Server
{
	public:
		Server(WebServer& webserver, Epoll& epoll);
		~Server(void);
	private:
		Server(void);
		Server(const Server& other);
		Server&	operator=(const Server& other);

	public:
		void		createClient(int listening_fd);
		void		deleteClient(int client_fd);
	
	private:
		WebServer&					_webserver;
		Epoll&						_epoll;
		std::vector<std::string>	_socket_addr;
		std::vector<ServerSocket*>	_listening_sockets;
		std::map<int, Client*>		_clients;

		

		void	_open_listening_sockets(void);

	public:
		class	NoListeningSocketsException : public std::exception
		{
			const char*	what(void) const throw();
		};
};

#endif
