#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include "ISocket.hpp"
# include <string>

class	Server;

class	ServerSocket : public ISocket
{
	public:
		ServerSocket(Server& server, std::string& socket_addr);
		~ServerSocket(void);
	private:
		ServerSocket(void);
		ServerSocket(const ServerSocket& other);
		ServerSocket&	operator=(const ServerSocket& other);

	public:
		void	handleEvent(void);

	private:
		Server&	_server;		
		int		_fd;

		void	_open_socket(std::string& ip, std::string& port);
	
	public:
		class	GaiException : public std::exception
		{
			const char*	what(void) const throw();
		};
		class	SocketCreateException : public std::exception
		{
			const char*	what(void) const throw();
		};
		class	BindException : public std::exception
		{
			const char*	what(void) const throw();
		};
};

#endif
