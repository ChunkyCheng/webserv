#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include "Client.hpp"

class	Server
{
	public:
		Server(int epollfd);
	
	private:
		Server(void);
		Server(const Server& other);
		Server&	operator=(const Server& other);

	public:
		~Server(void);

	protected:
	private:
		const int			_epollfd;
		std::string			_port;
		int					_listening_socket;
		std::vector<Client>	_clients;

		void	_open_listening_sockets(void);
		void	_add_to_epoll(void);
	
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
