#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "socket.h"
#include "Server.hpp"
#include "../../httpProtocol/HttpRequest.hpp"
#include "../../httpProtocol/HttpResponse.hpp"

enum ResponseState
{
	RES_HEADERS,
	RES_BODY,
	RES_FINISHED
};

enum RequestState
{
	REQ_READING_HEADERS,
	REQ_READING_BODY,
	REQ_COMPLETE,
	REQ_ERROR
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
		RequestState	getState() const;
		bool			checkRequestComplete(void) const;
		void			continueBuildResponse(void);
		bool			checkResponseComplete(void) const;
		void			processReqData(void);
		void			buildResponseData(void);
		void			reset();

	protected:
	private:
		const Server&	_server;
		std::string&	_request_buff;
		std::string&	_response_buff;
		HttpRequest		_httpRequest;
		HttpResponse	_httpResponse;
		RequestState	_req_state;
		ResponseState	_res_state;
};

#endif
