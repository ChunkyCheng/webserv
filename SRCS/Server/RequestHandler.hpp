#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "socket.h"
#include "Server.hpp"
#include "Location.hpp"
#include "../httpProtocol/HttpRequest.hpp"
#include "../httpProtocol/HttpResponse.hpp"
#include "../httpProtocol/HttpStatus.hpp"

enum ResponseState
{
	RES_HEADERS,
	RES_BODY,
	RES_CGI_BODY,
	RES_FINISHED
};

enum RequestState
{
	REQ_READING_HEADERS,
	REQ_READING_BODY,
	REQ_COMPLETE,
	// REQ_WAITING_CGI,
	// REQ_READY_TO_SEND,
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

		const Location*	matchLocation(const std::string& request_uri, const std::vector<Location>& location);
		bool			readFileContent(const std::string &file_path, std::string& out_content);
		std::string		getErrorPagePath(HttpStatus error_code);
		const Location* getDefaultLocation(void) const;
		std::string		getNormalPagePath(void);
		void			handleGetMethod(const std::string& physical_path);
		std::string		generateAutoindex(const std::string& physical_path, const std::string& uri);
		HttpStatus		validateRequestLocation(void);
		HttpStatus		resolveInitialStatus(void);
		void			evaluateConnectionState(HttpStatus status);
		HttpStatus		executeMethod(void);
		void			buildErrorOrRedirectResponse(HttpStatus status);
		void			assembleFinalBuffer();
		void			handleDeleteMethod(const std::string& physical_path);
		bool			isCgiExtension(std::string& ext);
		void			handlePostMethod(const std::string& physical_path);


	public:
		bool			checkRequestComplete(void) const;
		bool			checkResponseComplete(void) const;
		void			processReqData(void);
		void			continueBuildResponse(void);
		void			buildResponseData(void);
		void			reset(void);

	protected:
	private:
		const Server&	_server;
		std::string&	_request_buff;
		std::string&	_response_buff;
		HttpRequest		_httpRequest;
		HttpResponse	_httpResponse;
		RequestState	_req_state;
		ResponseState	_res_state;
		HttpStatus		_handler_error_code;
		const Location*	_location;
		bool			_should_close_connection;
		std::ifstream	_file_stream;
		std::string		_redirect_target;
		bool			_is_cgi;

};

#endif
