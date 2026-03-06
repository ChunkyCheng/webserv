/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:29 by yelu              #+#    #+#             */
/*   Updated: 2026/03/06 16:12:52 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "request.hpp"

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

void HttpRequest::parse(const char *rawText)
{
	std::stringstream	ss(rawText);
	std::string			line;
	std::string			extra;

	// getline(stream, str, delim); input stream from where characters are read
	// str where the input is stored after being read from the stream
	// delim is optional, default is '\n'
	std::getline(ss, line);
	if (line.empty() || line[line.size() - 1] != '\r')
	{
		std::cout << "400 Bad Request: Missing CR\n";
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
		std::cout << "405 Method Not Allowed\n";
		return ;
	}
	if (_version != "HTTP/1.1" && _version != "HTTP/1.0")
	{
		std::cout << "505 HTTP Version Not Supported\n";
		return ;
	}
	std::cout << _method << "\n" << _path << "\n" << _version << "\n"; // debug
}

// NGINX, the state machine
// State 1: Look for Method. If I see 'G', then 'E', then 'T', then a space, I am now in State 2.
// State 2: Look for Path. Keep reading until I hit a space.
// State 3: Look for HTTP Version... and so on.

// Request-Line grammar = METHOD SP URI SP HTTP-VERSION CRLF

// request-line
//    method = GET
//    request-target = /index.html
//    version = HTTP/1.1

// header-field
//    Host: example.com

// header-field
//    Connection: close

// (empty line)

// message-body = none