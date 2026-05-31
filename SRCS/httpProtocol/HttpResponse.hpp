#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <iostream>
#include <map>
#include "HttpStatus.hpp"
#include <string>
#include <fstream>

class HttpRequest;

class HttpResponse
{
	private:
		std::string								_version;
		HttpStatus								_status_code;
		std::string								_reason_phrase;
		std::multimap<std::string, std::string>	_headers;
		std::string								_body;

	public:
		HttpResponse();
		HttpResponse(const HttpResponse& other);
		HttpResponse& operator=(const HttpResponse& other);
		~HttpResponse();

		void		setStatusCode(HttpStatus status_code);
		void		overwriteHeader(const std::string& key, const std::string& value);
		void		addHeader(const std::string& key, const std::string& value);
		void		appendHeader(const std::string& key, const std::string& value);

		const std::string&	getReasonPhrase() const;
		std::string			getFormattedHeaders() const;
		const std::string&	getBody() const;
		std::string			getMimeType(const std::string& path);

		void				setBody(const std::string& body);

		void				buildErrorPage(HttpStatus error_code, const std::string& body_content);
		void				buildNormalHeaders(std::streamsize file_size, const std::string& physical_path);
		void				buildRedirectHeaders(const std::string& target, HttpStatus code);
		void				buildAutoIndexResponse(const std::string& autoindex_body);
		std::string			sizeToString(size_t number) const;
		void				reset();
};

#endif