#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "socket.h"
# include "ClientSocket.hpp"

class	RequestHandler;

class	Client
{
	public:
		Client(int socket_fd, Server& server);
		~Client(void);
	private:
		Client(void);
		Client(const Client& other);
		Client&	operator=(const Client& other);

	public:
		ClientSocket&	getSocket(void);
		void			recvMessage(void);
		void			sendMessage(void);

	private:
		ClientSocket	_socket;
		Server&			_server;
		RequestHandler	_requestHandler;
		std::string		_request_buff;
		std::string		_response_buff;
};

#endif
