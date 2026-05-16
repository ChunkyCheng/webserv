/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akok <akok@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:41 by yelu              #+#    #+#             */
/*   Updated: 2026/05/16 12:08:29 by akok             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "HttpStatus.hpp"

class HttpRequest
{
	private:
		// Track state
		HttpStatus		_error_code;

		// Protocol identifiers
		std::string		_method; // First line of the request, e.g., "GET", "POST", etc.
		std::string		_path; // The path requested, e.g., "/index.html"
		std::string		_version; // The HTTP version, e.g., "HTTP/1.1"

		// Metadata
		std::map<std::string, std::string>	_headers; // Key-value pairs of headers, e.g., "Host: example.com"
		std::string 						_body; // After the empty line \r\n\r\n, whatever is left is the Body.
		size_t								_content_length;
		bool								_is_chunked;
		bool								_keep_alive;

		bool	tokenizeAndParse(std::string& raw_headers);
		bool	parseRequestLine(std::string& line);
		bool	parseHeaderLine(const std::string& line);
		bool	setupBodyType();
		bool	parseContentLength();

	public:
		HttpRequest();
		HttpRequest(const HttpRequest& other);
		HttpRequest& operator=(const HttpRequest& other);
		~HttpRequest();

		bool	parseHeaders(std::string& req_buff);
		bool	parseBody(std::string& req_buff);
		void	reset();

		bool	hasError() const;
		bool	hasBody() const;
		bool	wantsKeepAlive() const;

		HttpStatus							getError() const;
		const std::string& 					getMethod() const;
		const std::string& 					getPath() const;
		const std::string& 					getVersion() const;
		std::map<std::string, std::string> 	getHeaders() const;
		std::map<std::string, std::string> 	getCookies() const;
		size_t								getContentLength() const;
		const std::string& 					getBody() const;
};

#endif
