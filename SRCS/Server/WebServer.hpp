#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <stdexcept>
# include <vector>
# include "Epoll.hpp"
# include "Server.hpp"
# include "Client.hpp"

class	WebServer
{
	public:
		WebServer(void);
		~WebServer(void);
	private:
		WebServer(const WebServer& other);
		WebServer&	operator=(const WebServer& other);
	
	public:
		void	createClient(int fd, Server& server);
		void	runServerLoop(void);
		
		static void	stopServerLoop(void);

	private:
		Epoll					_epoll;
		std::vector<Server*>	_servers;
		std::vector<Client*>	_clients;

		static bool				_runServer;
};

#endif
