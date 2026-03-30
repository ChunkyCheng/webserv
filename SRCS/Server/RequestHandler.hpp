#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "socket.h"
#include "Server.hpp"
#include "../../httpProtocol/HttpRequest.hpp"

enum HandlerState
{
	STATE_READING_HEADERS,
	STATE_READING_BODY,
	STATE_COMPLETE,
	STATE_ERROR
};

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
		HandlerState	getState() const;
		bool			checkRequestComplete(void) const;
		void			continueBuildResponse(void);
		bool			checkResponseComplete(void) const;
		void			processData(void);
		void			reset();

	protected:
	private:
		const Server&	_server;
		std::string&	_request_buff;
		std::string&	_response_buff;
		HttpRequest		_httpRequest;
		HandlerState	_state;
};

#endif
