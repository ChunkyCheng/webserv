#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "socket.h"
# include "Epoll.hpp"

class	WebServer
{
	public:
		WebServer(void);
		WebServer(std::string config_path);
		~WebServer(void);
	private:
		WebServer(const WebServer& other);
		WebServer&	operator=(const WebServer& other);
	
	public:
		Epoll&	getEpoll(void);

		void	runServerLoop(void);
		
		static void	stopServerLoop(void);

	private:
		Epoll					_epoll;
		std::vector<Server*>	_servers;

		static bool				_runServer;

		void	_initServer(const std::string config_path);
};

#endif
