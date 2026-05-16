#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "socket.h"
#include "Server.hpp"
#include "Location.hpp"
#include "../httpProtocol/HttpRequest.hpp"
#include "../httpProtocol/HttpResponse.hpp"
#include "../httpProtocol/HttpStatus.hpp"
#include "../CGIHandler/CGIHandler.hpp"

enum ResponseState
{
	RES_HEADERS,
	RES_BODY,
	RES_CGI_WAITING,
	RES_CGI_BODY,
	RES_FINISHED
};

enum RequestState
{
	REQ_READING_HEADERS,
	REQ_READING_BODY,
	// REQ_CGI,
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

		const Location*	matchLocation(const std::string& request_uri, const std::vector<Location>& location);
		bool			readFileContent(const std::string &file_path, std::string& out_content);
		std::string		getErrorPagePath(HttpStatus error_code);
		const Location* getDefaultLocation(void) const;
		std::string		getNormalPagePath(void);
		bool			isCgiRequest(const std::string& physical_path, Config::s_cgi_info& info) const;
		void			handleCgiMethod(const std::string& physical_path);
		void			handleGetMethod(const std::string& physical_path);
		void			handlePostMethod(const std::string& physical_path);
		void			handleDeleteMethod(const std::string& physical_path);
		std::string		generateAutoindex(const std::string& physical_path, const std::string& uri);
		HttpStatus		validateRequestLocation(void);
		HttpStatus		resolveInitialStatus(void);
		void			evaluateConnectionState(HttpStatus status);
		HttpStatus		executeMethod(void);
		void			buildErrorOrRedirectResponse(HttpStatus status);
		void			assembleFinalBuffer();


	public:
		bool			checkRequestComplete(void) const;
		bool			checkResponseComplete(void) const;
		bool			isCgiWaiting(void) const;
		void			processReqData(void);
		void			continueBuildResponse(void);
		void			buildResponseData(void);
		void		reset(void);

		// Expose Set-Cookie headers from the current response
		std::vector<std::string> getSetCookieHeaders() const;

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
		int				_cgi_process_pid;
		time_t			_cgi_start_time;
		CGIHandler*		_cgi_handler;
		static const int CGI_TIMEOUT = 5;

};

#endif
