/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:44:52 by yelu              #+#    #+#             */
/*   Updated: 2026/04/16 15:59:12 by yelu             ###   ########.fr       */
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
			break;
		case NOT_IMPLEMENTED:
			_reason_phrase = "Not Implemented";
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

std::string HttpResponse::getFormattedHeaders() const
{
	std::string output = "";
	output += _version + " " + sizeToString(static_cast<size_t>(_status_code)) + " " + _reason_phrase + "\r\n";
	std::multimap<std::string, std::string>::const_iterator it;
	for (it = _headers.begin(); it != _headers.end(); ++it)
	{
		output += it->first + ": " + it->second + "\r\n";
	}
	output += "\r\n";
	return (output);
}

void	HttpResponse::buildErrorPage(HttpStatus error_code, const std::string& error_file_path)
{
	setStatusCode(error_code);
	_body.clear();
	if (!error_file_path.empty())
	{
		std::ifstream file(error_file_path.c_str(), std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			char buffer[4096];
			while (file.read(buffer, sizeof(buffer)))
			{
				_body.append(buffer, file.gcount());
			}
			if (file.gcount() > 0)
			{
				_body.append(buffer, file.gcount());
			}
			file.close();
		}
	}
	if (_body.empty())
	{
		std::string error_str = sizeToString(error_code);
		_body = "<html>\r\n";
		_body += "<head><title>Error " + error_str + "</title></head>\r\n";
        _body += "<center><h1>" + _reason_phrase + "</h1></center>\r\n";
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

std::string HttpResponse::toString()
{
	std::string response_str = "";
	response_str += _version;
	response_str += " ";
	response_str += sizeToString(static_cast<size_t>(_status_code));
	response_str += " ";
	response_str += _reason_phrase;
	response_str += "\r\n";
	std::cout << "Response str before headers: " << response_str << "\n";

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
	std::cout << "Response str after body: " << response_str << "\n";
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
