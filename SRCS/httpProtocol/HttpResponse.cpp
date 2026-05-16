/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:44:52 by yelu              #+#    #+#             */
/*   Updated: 2026/04/29 17:40:17 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include <cstdlib>

HttpResponse::HttpResponse()
	: _version("HTTP/1.1"),
	_status_code(NONE)
{}

HttpResponse::HttpResponse(const HttpResponse& other)
	: _version(other._version),
	_status_code(other._status_code),
	_reason_phrase(other._reason_phrase),
	_headers(other._headers),
	_body(other._body)
{}

HttpResponse& HttpResponse::operator=(const HttpResponse& other)
{
	if (this != &other)
	{
		_version = other._version;
		_status_code = other._status_code;
		_reason_phrase = other._reason_phrase;
		_headers = other._headers;
		_body = other._body;
	}
	return (*this);
}

void	HttpResponse::reset()
{
	_status_code = NONE;
	_reason_phrase = "";
	_headers.clear();
	_body.clear();
}

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
		case MOVED_PERMANENTLY:
			_reason_phrase = "Moved Permanently";
			break;
		case FOUND:
			_reason_phrase = "Found";
			break;
		case SEE_OTHER:
			_reason_phrase = "See Other";
			break;
		case TEMPORARY_REDIRECT:
			_reason_phrase = "Temporary Redirect";
			break;
		case PERMANENT_REDIRECT:
			_reason_phrase = "Permanent Redirect";
			break;
		case BAD_REQUEST:
			_reason_phrase = "Bad Request";
			break;
		case FORBIDDEN:
			_reason_phrase = "Forbidden";
			break;
		case NOT_FOUND:
			_reason_phrase = "Not Found";
			break;
		case METHOD_NOT_ALLOWED:
			_reason_phrase = "Method Not Allowed";
			break;
		case PAYLOAD_TOO_LARGE:
			_reason_phrase = "Payload Too Large";
			break;
		case INTERNAL_SERVER_ERROR:
			_reason_phrase = "Internal Server Error";
			break;
		case NOT_IMPLEMENTED:
			_reason_phrase = "Not Implemented";
			break;
		case HTTP_VERSION_NOT_SUPPORTED:
			_reason_phrase = "HTTP Version Not Supported";
			break;
		default:
			_status_code = INTERNAL_SERVER_ERROR;
			_reason_phrase = "Internal Server Error";
			std::cerr << "[HTTP ERROR] Unhandled HttpStatus passed to setStatusCode\n";
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

std::string HttpResponse::sizeToString(size_t number) const
{
	if (number == 0)
		return ("0");
	std::string result = "";
	while (number > 0)
	{
		result.insert(result.begin(), (number % 10) + '0');
		number /= 10;
	}
	return (result);
}

const std::string& HttpResponse::getReasonPhrase() const
{
	return (_reason_phrase);
}

std::vector<std::string> HttpResponse::getHeaderValues(const std::string& key) const
{
	std::vector<std::string> values;
	std::string lower_key = key;
	for (size_t i = 0; i < lower_key.size(); ++i) lower_key[i] = std::tolower(lower_key[i]);
	for (std::multimap<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		std::string h = it->first;
		std::string hl = h;
		for (size_t i = 0; i < hl.size(); ++i) hl[i] = std::tolower(hl[i]);
		if (hl == lower_key)
			values.push_back(it->second);
	}
	return (values);
}

void	HttpResponse::buildNormalHeaders(std::streamsize file_size, const std::string& physical_path)
{
	setStatusCode(OK);
	addHeader("Content-Type", getMimeType(physical_path));
	addHeader("Content-Length", sizeToString(file_size));
}

void	HttpResponse::buildRedirectHeaders(const std::string& target, HttpStatus code)
{
		setStatusCode(code);
		addHeader("Location", target);
		addHeader("Content-Length", "0");
}

void	HttpResponse::buildErrorPage(HttpStatus error_code, const std::string& body_content)
{
	setStatusCode(error_code);
	_body.clear();
	_body = body_content;
	if (_body.empty())
	{
		std::string error_str = sizeToString(error_code);
		_body = "<html>\r\n";
		_body += "<head><title>Error " + error_str + "</title></head>\r\n";
		_body += "<center><h1>Error " + error_str + _reason_phrase + "</h1></center>\r\n";
		_body += "</body>\r\n";
		_body += "</html>\r\n";
	}
	addHeader("Content-Type", "text/html");
	addHeader("Content-Length", sizeToString(_body.length()));
}

void	HttpResponse::setBody(const std::string &body)
{
	_body = body;
}

const std::string& HttpResponse::getBody() const
{
	return (_body);
}

void	HttpResponse::buildAutoIndexResponse(const std::string& autoindex_body)
{
	setStatusCode(OK);
	_body = autoindex_body;
	std::string content_length = sizeToString(_body.length());
	addHeader("Content-Type", "text/html");
	addHeader("Content-Length", content_length);
}

std::string HttpResponse::getFormattedHeaders() const
{
	std::string response_str = "";
	response_str += _version;
	response_str += " ";
	response_str += sizeToString(static_cast<size_t>(_status_code));
	response_str += " ";
	response_str += _reason_phrase;
	response_str += "\r\n";

	std::multimap<std::string, std::string>::const_iterator it;
	for (it = _headers.begin(); it != _headers.end(); ++it)
	{
		response_str += it->first;
		response_str += ": ";
		response_str += it->second;
		response_str += "\r\n";
	}
	response_str += "\r\n";
	std::cout << response_str << "\n";
	return (response_str);
}

std::string HttpResponse::getMimeType(const std::string& path)
{
	size_t dotPos = path.find_last_of('.');
	if (dotPos == std::string::npos)
		return "text/plain"; 

	std::string ext = path.substr(dotPos);

	if (ext == ".html" || ext == ".htm") return "text/html";
	if (ext == ".css") return "text/css";
	if (ext == ".js") return "text/javascript";
	
	if (ext == ".png") return "image/png";
	if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
	if (ext == ".gif") return "image/gif";
	if (ext == ".ico") return "image/x-icon";
	
	if (ext == ".mp4") return "video/mp4";
	if (ext == ".mp3") return "audio/mpeg";

	return "text/plain";
}

