/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:41 by yelu              #+#    #+#             */
/*   Updated: 2026/03/29 13:58:24 by yelu             ###   ########.fr       */
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

enum HttpReqStatus
{
	OK = 200,
	BAD_REQUEST = 400,
	NOT_FOUND = 404,
	INTERNAL_SERVER_ERROR = 500
};

class HttpRequest
{
	private:
		HttpReqStatus	_status_code;
		httpRequestState	_state;
		std::string		_method; // First line of the request, e.g., "GET", "POST", etc.
		std::string 	_path; // The path requested, e.g., "/index.html"
		std::string 	_version; // The HTTP version, e.g., "HTTP/1.1"
		std::map<std::string, std::string> _headers; // Key-value pairs of headers, e.g., "Host: example.com"
		std::string 	_body; // After the empty line \r\n\r\n, whatever is left is the Body.
		size_t			_content_length;

	public:
		HttpRequest();
		void	parse(const std::string& request_buff);
		bool	hasCompleteHeaders();

		std::string& 		getMethod() const;
		std::string& 		getPath() const;
		std::string& 		getVersion() const;
		httpRequestState& 	getState() const;
		std::map<std::string, std::string> getHeaders() const;
		size_t				getContentLength() const;
};
