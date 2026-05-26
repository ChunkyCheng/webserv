/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:29 by yelu              #+#    #+#             */
/*   Updated: 2026/05/14 02:48:52 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
	: _error_code(NONE),
	_chunk_state(CHUNK_SIZE),
	_chunk_size(0),
	_chunk_bytes_read(0),
	_content_length(0),
	_is_chunked(false),
	_keep_alive(true)
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

size_t	HttpRequest::getContentLength() const
{
	return (_content_length);
}

const std::string& HttpRequest::getBody() const
{
	return (_body);
}

bool	HttpRequest::hasBody() const
{
	if (_headers.find("content-length") != _headers.end())
		return (true);
	if (_headers.find("transfer-encoding") != _headers.end())
		return (true);
	return (false);
}

bool	HttpRequest::hasError() const
{
	if (_error_code != NONE)
		return (true);
	else
		return (false);
}

HttpStatus	HttpRequest::getError() const
{
	return (_error_code);
}

bool	HttpRequest::wantsKeepAlive() const
{
	return (_keep_alive);
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
			if (line[0] == ' ' || line[0] == '\t')
			{
				_error_code = BAD_REQUEST;
				return (false);
			}
			if (!parseHeaderLine(line))
			{
				return (false);
			}
		}
		pos = end + 2;
	}
	std::map<std::string, std::string>::iterator host_it = _headers.find("host");
	if (host_it == _headers.end())
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	std::map<std::string, std::string>::iterator conn_it = _headers.find("connection");
	if (conn_it != _headers.end())
	{
		if (conn_it->second.find("close") != std::string::npos)
		{
			_keep_alive = false;
		}
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
	for (size_t i = 0; i < _path.length(); ++i)
	{
		if (_path[i] == ' ' || _path[i] == '\t')
		{
			_error_code = BAD_REQUEST;
			return (false);
		}
	}
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
	size_t start = value.find_first_not_of(" \t");
	if (start == std::string::npos)
		value = "";
	else
	{
		size_t end = value.find_last_not_of(" \t");
		value = value.substr(start, end - start + 1);
	}
	if (_headers.find(key) != _headers.end())
	{
		if (key == "host" || key == "content-length" || key == "transfer-encoding")
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

bool	HttpRequest::parseBody(std::string& req_buff, size_t max_body_size)
{
	if (_is_chunked)
		return (parseChunkedBody(req_buff, max_body_size));
	else
	{
		size_t bytes_needed = _content_length - _body.size();
		size_t bytes_to_take = std::min(bytes_needed, req_buff.size());
		if (_body.size() + bytes_to_take > max_body_size)
		{
			_error_code = PAYLOAD_TOO_LARGE;
			return (false);
		}
		if (bytes_to_take > 0)
		{
			_body.append(req_buff, 0, bytes_to_take);
			req_buff.erase(0, bytes_to_take);
		}
		if (_body.size() == _content_length)
		{
			return (true);
		}
		return (false);
	}
}

bool	HttpRequest::setupBodyType()
{
	bool	has_te = (_headers.find("transfer-encoding") != _headers.end());
	bool	has_cl = (_headers.find("content-length") != _headers.end());

	if (has_te && has_cl)
	{
		_error_code = BAD_REQUEST;
		return (false);
	}
	if (has_te)
	{
		std::string te_val = _headers["transfer-encoding"];
		for (size_t i = 0; i < te_val.length(); ++i)
		{
			te_val[i] = std::tolower(te_val[i]);
		}
		if (te_val.find("chunked") != std::string::npos)
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
// Loop runs as long as three conditions are true: incoming buffer has data, haven't finished parsing (CHUNK_COMPLETE), and no errors have occured
// Since chunk encoding format is fixed, we always start with checking size first, thus, CHUNK SIZE is triggered
// 
bool	HttpRequest::parseChunkedBody(std::string& req_buff, size_t max_body_size)
{
	while (!req_buff.empty() && _chunk_state != CHUNK_COMPLETE && _chunk_state != CHUNK_ERROR)
	{
		switch (_chunk_state)
		{
			case CHUNK_SIZE:
			{
				size_t pos = req_buff.find("\r\n");
				if (pos == std::string::npos)
				{
					return (false);
				}
				std::string chunked_str = req_buff.substr(0, pos);
				size_t semi = chunked_str.find(';');
				if (semi != std::string::npos)
				{
					chunked_str = chunked_str.substr(0, semi);
				}
				if (chunked_str.empty() || chunked_str.length() > 16)
				{
					_chunk_state = CHUNK_ERROR;
					_error_code = BAD_REQUEST;
					return (false);
				}
				for (size_t i = 0; i < chunked_str.length(); ++i)
				{
					if (!std::isxdigit(static_cast<unsigned char>(chunked_str[i])))
					{
						_chunk_state = CHUNK_ERROR;
						_error_code = BAD_REQUEST;
						return (false);
					}
				}
				std::stringstream ss;
				ss << std::hex << chunked_str;
				ss >> _chunk_size;
				if (ss.fail())
				{
					_chunk_state = CHUNK_ERROR;
					_error_code = BAD_REQUEST;
					return (false);
				}
				req_buff.erase(0, pos + 2);
				_chunk_bytes_read = 0;
				if (_chunk_size == 0)
				{
					_chunk_state = CHUNK_TRAILER;
				}
				else
				{
					_chunk_state = CHUNK_DATA;
				}
				break;
			}

			case CHUNK_DATA:
			{
				size_t bytes_needed = _chunk_size - _chunk_bytes_read;
				size_t bytes_to_take = std::min(bytes_needed, req_buff.size());

				if (bytes_to_take > 0)
				{
					if (_body.size() + bytes_to_take > max_body_size)
					{
						_chunk_state = CHUNK_ERROR;
						_error_code = PAYLOAD_TOO_LARGE;
						return (false);
					}
					_body.append(req_buff, 0, bytes_to_take);
					req_buff.erase(0, bytes_to_take);
					_chunk_bytes_read += bytes_to_take;
				}
				if (_chunk_bytes_read == _chunk_size)
				{
					_chunk_state = CHUNK_CRLF;
				}
				else
				{
					return (false);
				}
				break;
			}

			case CHUNK_CRLF:
			{
				if (req_buff.length() < 2)
				{
					return (false);
				}
				if (req_buff.substr(0, 2) != "\r\n")
				{
					_chunk_state = CHUNK_ERROR;
					_error_code = BAD_REQUEST;
					return (false);
				}
				req_buff.erase(0, 2);
				_chunk_state = CHUNK_SIZE;
				break;
			}
			
			case CHUNK_TRAILER:
			{
				size_t pos = req_buff.find("\r\n");
				if (pos == std::string::npos)
				{
					return (false);
				}
				if (pos == 0)
				{
					req_buff.erase(0, 2);
					_content_length = _body.size();
					_chunk_state = CHUNK_COMPLETE;
				}
				else
				{
					req_buff.erase(0, pos + 2);
				}
				break;
			}
			default:
				break;
		}
	}
	return (_chunk_state == CHUNK_COMPLETE);
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
	_is_chunked = false;
	_keep_alive = true;
	_chunk_bytes_read = 0;
	_chunk_size = 0;
	_chunk_state = CHUNK_SIZE;
}
