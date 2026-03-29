#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

#include "socket.h"
#include "Server.hpp"

class	RequestHandler
{
	public:
		RequestHandler(Server& server, std::string& req_buff, std::string& res_buff);
		~RequestHandler(void);
	private:
		RequestHandler(void);
		RequestHandler(const RequestHandler& other);
		RequestHandler&	operator=(const RequestHandler& other);

	public:
		bool	checkRequestComplete(void) const;
		void	continueBuildResponse(void);
		bool	checkResponseComplete(void) const;

	protected:
	private:
		const Server&	_server;
		std::string&	_request_buff;
		std::string&	_response_buff;
};

#endif
