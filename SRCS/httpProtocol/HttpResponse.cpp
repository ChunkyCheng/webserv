/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:44:52 by yelu              #+#    #+#             */
/*   Updated: 2026/04/12 21:28:52 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include <cstdlib>

HttpResponse::HttpResponse()
	: _version("HTTP/1.1"),
	_status_code(NONE)
{}

HttpResponse::~HttpResponse() {}

void	HttpResponse::setStatusCode(HttpStatus status_code)
{
	_status_code = status_code;
	switch(status_code)
	{
		case OK:
			_reason_phrase = "OK";
			break;
		case NO_CONTENT:
			_reason_phrase = "No Content";
			break;
		case BAD_REQUEST:
			_reason_phrase = "Bad Request";
			break;
		case NOT_FOUND:
			_reason_phrase = "Not Found";
			break;
		case INTERNAL_SERVER_ERROR:
			_reason_phrase = "Internal Server Error";
		case NOT_IMPLEMENTED:
			_reason_phrase = "Not Implemented";
			break;
		default:
			_reason_phrase = "Unknown";
			break;
	}
}

void	HttpResponse::overwriteHeader(const std::string& key, const std::string& value)
{
	_headers.erase(key);
	_headers.insert(std::make_pair(key, value));
}

void	HttpResponse::addHeader(const std::string& key, const std::string& value)
{
	_headers.insert(std::make_pair(key, value));
}

void	HttpResponse::appendHeader(const std::string& key, const std::string& value)
{
	std::multimap<std::string, std::string>::iterator it = _headers.find(key);

	if (it != _headers.end())
	{
		it->second += ", " + value;
	}
	else
	{
		_headers.insert(std::make_pair(key, value));
	}
}

std::string HttpResponse::sizeToString(size_t number)
{
	if (number == 0)
		return (0);
	std::string result = "";
	while (number > 0)
	{
		result.insert(result.begin(), (number % 10) + '0');
		number /= 10;
	}
	return (result);
}

void	HttpResponse::buildErrorPage(HttpStatus error_code)
{
	setStatusCode(error_code);
	std::string html = "<html>\r\n";
				html += "<head>Error</head>\r\n";
				html += "</html>\r\n";
	_body = html;
	addHeader("Content-Type", "text/html");
	std::string length_str = sizeToString(_body.length());
	addHeader("Content-Length", length_str);
}

void	HttpResponse::setBody(const std::string &body)
{
	_body = body;
}

const std::string& HttpResponse::getBody() const
{
	return (_body);
}

std::string HttpResponse::toString()
{
	std::string response_str = "";
	response_str += _version;
	response_str += " ";
	response_str += sizeToString(static_cast<size_t>(_status_code));
	response_str += " ";
	response_str += _reason_phrase;
	response_str += "\r\n";

	std::multimap<std::string, std::string>::iterator it;
	for (it = _headers.begin(); it != _headers.end(); ++it)
	{
		response_str += it->first;
		response_str += ": ";
		response_str += it->second;
		response_str += "\r\n";
	}
	response_str += "\r\n";
	response_str += _body;
	return (response_str);
}
