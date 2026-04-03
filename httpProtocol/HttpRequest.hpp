/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:41 by yelu              #+#    #+#             */
/*   Updated: 2026/04/03 15:40:53 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sstream>
#include <map>

enum httpRequestState
{
	READING_HEADER,
	READING_BODY,
	FINISHED,
	ERROR
};

enum HttpErrStatus
{
	NONE = 0,
	OK = 200,
	BAD_REQUEST = 400,
	NOT_FOUND = 404,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	HTTP_VERSION_NOT_SUPPORTED = 505
};

class HttpRequest
{
	private:
		HttpRequest(const HttpRequest& other);
		HttpRequest& operator=(const HttpRequest& other);

		// Track state
		HttpErrStatus		_error_code;
		httpRequestState	_state;

		// Protocol identifiers
		std::string		_method; // First line of the request, e.g., "GET", "POST", etc.
		std::string 	_path; // The path requested, e.g., "/index.html"
		std::string 	_version; // The HTTP version, e.g., "HTTP/1.1"

		// Metadata
		std::map<std::string, std::string> _headers; // Key-value pairs of headers, e.g., "Host: example.com"
		std::string 	_body; // After the empty line \r\n\r\n, whatever is left is the Body.
		size_t			_content_length;
		bool			_is_chunked;

	public:
		HttpRequest();
		~HttpRequest();

		bool	parseHeaders(std::string& req_buff);
		bool	tokenizeAndParse(std::string& raw_headers);
		bool	parseRequestLine(std::string& line);
		bool	parseHeaderLine(const std::string& line);
		void	reset();

		bool	hasError(void);
		bool	hasBody(void);

		const std::string& 		getMethod() const;
		const std::string& 		getPath() const;
		const std::string& 		getVersion() const;
		httpRequestState 		getState() const;
		std::map<std::string, std::string> getHeaders() const;
		size_t				getContentLength() const;
};
