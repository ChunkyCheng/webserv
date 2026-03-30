/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:29 by yelu              #+#    #+#             */
/*   Updated: 2026/03/29 14:01:25 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : _state(READING_HEADER) {}

RequestState HttpRequest::getState() const
{
	return (_state);
}


std::string HttpRequest::getMethod() const
{
	return (_method);
}
		
std::string HttpRequest::getPath() const
{
	return (_path);
}
		
std::string HttpRequest::getVersion() const
{
	return (_version);
}
		
std::map<std::string, std::string> HttpRequest::getHeaders() const
{
	return (_headers);
}

void HttpRequest::parse(const std::string& request_buff)
{
	std::stringstream	ss(request_buff);
	std::string			line, extra;

	// getline(stream, str, delim); input stream from where characters are read
	// str where the input is stored after being read from the stream
	// delim is optional, default is '\n'
	// --- Parse request line ---
	std::getline(ss, line);
	if (line.empty() || line[line.size() - 1] != '\r')
	{
		std::cout << "400 Bad Request: Missing CR\n"; // User sent gibberish
		return ;
	}
	else if (!line.empty() && line[line.size() - 1] == '\r')
	{
		line.erase(line.size() - 1);
	}
	std::stringstream requestLine(line);
	if (!(requestLine >> _method >> _path >> _version))
	{
		std::cout << "400 Bad Request: Invalid Request Line\n"; // Failed, didn't have three parts, return error code 400
		return ;
	}
	if (requestLine >> extra)
	{
		std::cout << "400 Bad Request: Extra parts\n";
		return ;
	}
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
	{
		std::cout << "405 Method Not Allowed\n"; //Tried to POST to a file that only allows GET
		return ;
	}
	if (_version != "HTTP/1.1" && _version != "HTTP/1.0")
	{
		std::cout << "505 HTTP Version Not Supported\n";
		return ;
	}
	std::cout << _method << "\n" << _path << "\n" << _version << "\n"; // debug

	// --- Parse headers ---
	while (std::getline(ss, line) && line != "\r")
	{
		if (line.empty() || line[line.size() - 1] != '\r')
		{
			std::cout << "400 Bad Request: Missing CR\n";
			return ;
		}
		else if (!line.empty() && line[line.size() - 1] == '\r')
		{
			line.erase(line.size() - 1);
		}
		size_t colon = line.find(':');
		if (colon == std::string::npos)
		{
			std::cout << "400 Bad Request: Invalid Header\n";
			return ;
		}
		std::string key = line.substr(0, colon);
		std::cout << key << "\n";
		std::string value = line.substr(colon + 1);
		value.erase(0, value.find_first_not_of(" \t")); // Search string from the beginning for the first char that is NOT a space or tab
		value.erase(value.find_last_not_of(" \t") + 1);
		std::cout << value << "\n";
		_headers[key] = value;
	}

	std::pair<std::string::iterator, std::string::iterator> range;
	_body = ss.str().substr(ss.tellg());
	_state = FINISHED;
}

bool HttpRequest::hasCompleteHeaders() const {
	return _raw_buffer.find("\r\n\r\n") != std::string::npos;
}
