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

HttpRequest::HttpRequest()
	: _status_code(0),
	_state(READING_HEADER),
	_content_length(0)

{}

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

bool	HttpRequest::parseHeaders(std::string &req_buff)
{
	size_t first_relevant = req_buff.find_first_not_of("\r\n");
	if (first_relevant == std::string::npos)
	{
		req_buff.clear();
		return (false);
	}
	if (first_relevant > 0)
		req_buff.erase(0, first_relevant);
	size_t boundary = req_buff.find("\r\n\r\n");
	if (boundary == std::string::npos)
		return (false);
	std::string raw_headers = req_buff.substr(0, boundary);
	if (!tokenizeAndParse(raw_headers))
		return (false);
	req_buff.erase(0, boundary + 4);
	return (true);
}

bool	HttpRequest::tokenizeAndParse(std::string& raw_headers)
{
	
	return (true);
}

bool	HttpRequest::hasBody(void)
{
	return (true);
}

bool	HttpRequest::hasError(void)
{
	return (true);
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

void	HttpRequest::reset()
{
	_method.clear();
	_path.clear();
	_version.clear();
	_headers.clear();
	_body.clear();
	_content_length = 0;
	_error_code = 0;
	_state = READING_HEADERS;
}
