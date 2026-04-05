/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:29 by yelu              #+#    #+#             */
/*   Updated: 2026/04/05 18:34:06 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
	: _error_code(NONE),
	_content_length(0),
	_is_chunked(false)
{}

HttpRequest::~HttpRequest() {}

const std::string& HttpRequest::getMethod() const
{
	return (_method);
}

const std::string& HttpRequest::getPath() const
{
	return (_path);
}

const std::string& HttpRequest::getVersion() const
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
	std::string raw_headers = req_buff.substr(0, boundary + 4);
	if (!tokenizeAndParse(raw_headers))
		return (false);
	req_buff.erase(0, boundary + 4);
	if (!setupBodyType())
		return (false);
	return (true);
}

bool	HttpRequest::tokenizeAndParse(std::string& raw_headers)
{
	size_t		pos = 0;
	size_t		end = 0;
	bool		is_first_line = true;
	std::string	line;

	while ((end = raw_headers.find("\r\n", pos)) != std::string::npos)
	{
		line = raw_headers.substr(pos, end - pos);
		if (is_first_line)
		{
			if (!parseRequestLine(line))
				return (false);
			is_first_line = false;
		}
		else if (!line.empty())
		{
			if (!parseHeaderLine(line))
				return (false);
			if (line[0] == ' ' || line[0] == '\t')
			{
				_error_code = BAD_REQUEST;
				return (false);
			}
		}
		pos = end + 2;
	}
	return (true);
}

// RFC rules: Exactly two spaces, no spaces in the target, case sensitive, No leading/trailing whitespace
bool	HttpRequest::parseRequestLine(std::string& line)
{
	size_t	sp1;
	size_t	sp2;

	if (line.empty() || line[0] == ' ' || line[0] == '\t')
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	sp1 = line.find(' ');
	if (sp1 == std::string::npos)
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	sp2 = line.find(' ', sp1 + 1);
	if (sp2 == std::string::npos)
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	_method = line.substr(0, sp1);
	_path = line.substr(sp1 + 1, sp2 - sp1 - 1);
	_version = line.substr(sp2 + 1);
	// Need to see if any whitespace in path, not implemented yet
	if (_method.empty() || _path.empty() || _version.empty())
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
	{
		_error_code = NOT_IMPLEMENTED;
		return (false);
	}
	if (_version != "HTTP/1.1")
	{
		_error_code = HTTP_VERSION_NOT_SUPPORTED;
		return (false);
	}
	return (true);
}

bool HttpRequest::parseHeaderLine(const std::string& line)
{
	size_t colon = line.find(':');
	if (colon == std::string::npos || colon == 0)
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	if (line[colon - 1] == ' ' || line[colon - 1] == '\t')
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	std::string key = line.substr(0, colon);
	std::string value = line.substr(colon + 1);
	for (size_t i = 0; i < key.length(); ++i)
	{
		if (key[i] <= 32 || key[i] == 127)
		{
			_error_code = BAD_REQUEST;
			return (false);
		}
		key[i] = std::tolower(key[i]);
	}
	if (_headers.find(key) != _headers.end())
	{
		if (key == "host")
		{
			_error_code = BAD_REQUEST;
			return (false);
		}
		_headers[key] += ", " + value;
	}
	else
		_headers[key] = value;
	return (true);
}

bool	HttpRequest::parseBody(std::string& req_buff)
{
	for (size_t i = 0; i <= _content_length; i++)
	{
		 
	}
}



bool	HttpRequest::hasBody()
{
	if (_headers.find("content-length") != _headers.end())
		return (true);
	if (_headers.find("transfer-encoding") != _headers.end())
		return (true);
	return (false);
}

bool	HttpRequest::hasError()
{
	if (_error_code != NONE)
		return (true);
	else
		return (false);
}

HttpErrStatus	HttpRequest::getError() const
{
	return (_error_code);
}

bool	HttpRequest::setupBodyType()
{
	// Transfer Encoding and Content length cannot exist at the same time
	bool	has_te = (_headers.find("transfer-encoding") != _headers.end());
	bool	has_cl = (_headers.find("content-length") != _headers.end());

	if (has_te && has_cl)
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	if (has_te)
	{
		if (_headers["transfer-encoding"].find("chunked") != std::string::npos)
		{
			_is_chunked = true;
			return (true);
		}
		_error_code = NOT_IMPLEMENTED;
		return (false);
	}
	if (has_cl)
		return (parseContentLength());
	return (true);
}

bool	HttpRequest::parseContentLength()
{
	std::string	value = _headers["content-length"];
	std::stringstream ss(value);
	long temp;

	ss >> temp;
	if (ss.fail() || !ss.eof() || temp < 0)
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	_content_length = static_cast<size_t>(temp);
	return (true);
}

void	HttpRequest::reset()
{
	_method.clear();
	_path.clear();
	_version.clear();
	_headers.clear();
	_body.clear();
	_content_length = 0;
	_error_code = NONE;
}
