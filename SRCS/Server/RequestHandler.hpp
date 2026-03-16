#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "socket.h"

class	RequestHandler
{
	public:
		RequestHandler(void);
		RequestHandler(const RequestHandler& other);
		RequestHandler&	operator=(const RequestHandler& other);
		~RequestHandler(void);

		bool	checkResponseReady(void);

	protected:
	private:
		const Server&	_server;
		std::string&	_request_buff;
		std::string&	_response_buff;
};

#endif
