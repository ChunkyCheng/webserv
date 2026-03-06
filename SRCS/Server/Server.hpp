#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include "Client.hpp"

class	Server
{
	public:
		Server(void);
	
	private:
		Server(const Server& other);
		Server&	operator=(const Server& other);

	public:
		~Server(void);

	protected:
	private:
		std::string			_port;
		int					_listening_socket;
		std::vector<Client>	_clients;

		void	_open_listening_sockets(void);
};

#endif
