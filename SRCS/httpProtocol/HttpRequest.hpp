/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:41 by yelu              #+#    #+#             */
/*   Updated: 2026/05/14 02:48:26 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "HttpStatus.hpp"

enum ChunkState
{
		CHUNK_SIZE,
		CHUNK_DATA,
		CHUNK_CRLF,
		CHUNK_TRAILER,
		CHUNK_COMPLETE,
		CHUNK_ERROR
};

class HttpRequest
{
	private:
		HttpRequest(const HttpRequest& other);
		HttpRequest& operator=(const HttpRequest& other);

		// Track state
		HttpStatus		_error_code;
		ChunkState		_chunk_state;
		size_t			_chunk_size;
		size_t			_chunk_bytes_read;

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
		bool	parseChunkedBody(std::string& req_buff, size_t max_body_size);

	public:
		HttpRequest();
		~HttpRequest();

		bool	parseHeaders(std::string& req_buff);
		bool	parseBody(std::string& req_buff, size_t max_body_size);
		void	reset();

		bool	hasError() const;
		bool	hasBody() const;
		bool	wantsKeepAlive() const;

		HttpStatus							getError() const;
		const std::string& 					getMethod() const;
		const std::string& 					getPath() const;
		const std::string& 					getVersion() const;
		std::map<std::string, std::string> 	getHeaders() const;
		size_t								getContentLength() const;
		const std::string&					getBody() const;
};

#endif