#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include "ISocket.hpp"

class	Client;

class	ClientSocket : public ISocket
{
	public:
		ClientSocket(Client& client, int fd);
		~ClientSocket(void);
	private:
		ClientSocket(void);
		ClientSocket(const ClientSocket& other);
		ClientSocket&	operator=(const ClientSocket& other);

	public:
		void	handleEvent(void);

	private:
		Client&		_client;
		const int	_fd;
};

#endif
