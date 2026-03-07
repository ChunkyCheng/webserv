#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <stdexcept>
# include <vector>
# include "Server.hpp"

class	WebServer
{
	public:
		WebServer(void);
		WebServer(const WebServer& other);
		WebServer&	operator=(const WebServer& other);
		~WebServer(void);

	protected:
	private:
		std::vector<Server*>	_servers;
		int						_epollfd;
	
	public:
		class	EpollCreateException : public std::exception
		{
			const char*	what(void) const throw();
		};
};

#endif
