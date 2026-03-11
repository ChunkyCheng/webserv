#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include "Client.hpp"
# include "ServerSocket.hpp"

class	WebServer;

class	Server
{
	public:
		Server(WebServer& webserver);
		~Server(void);
	private:
		Server(void);
		Server(const Server& other);
		Server&	operator=(const Server& other);

	public:
		WebServer&	getWebServer(void);

	private:
		WebServer&					_webserver;
		std::vector<std::string>	_socket_addr;
		std::vector<ServerSocket*>	_listening_sockets;

		void	_open_listening_sockets(void);
};

#endif
