#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include "ISocket.hpp"
# include "socket.h"

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
		int		getFd(void) const;
		void	handleEvent(int events);

	private:
		Client&		_client;
		const int	_fd;
};

#endif
