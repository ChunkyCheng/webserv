#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <stdexcept>
# include <vector>
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
		int		getEpollfd(void) const;
		void	createClient(int fd, Server& server);

	private:
		std::vector<Server*>	_servers;
		std::vector<Client*>	_clients;
		int						_epollfd;
	
	public:
		class	EpollCreateException : public std::exception
		{
			const char*	what(void) const throw();
		};
};

#endif
