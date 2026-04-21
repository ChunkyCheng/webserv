#include "RequestHandler.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

RequestHandler::RequestHandler(Server& server,
std::string& req_buff, std::string& res_buff)
	: _server(server),
	_request_buff(req_buff),
	_response_buff(res_buff),
	_httpRequest(),
	_req_state(REQ_READING_HEADERS),
	_res_state(RES_HEADERS),
	_handler_error_code(NONE),
	_location(NULL),
	_should_close_connection(false)
{
}

RequestHandler::~RequestHandler(void)
{
}

void	RequestHandler::reset(void)
{
	_req_state = REQ_READING_HEADERS;
	_res_state = RES_HEADERS;
	_httpRequest.reset();
	_httpResponse.reset();
}

bool	RequestHandler::checkRequestComplete(void) const
{
	return (_req_state == REQ_COMPLETE || _req_state == REQ_ERROR);
}

void	RequestHandler::continueBuildResponse(void)
{
	if (_res_state == RES_BODY)
	{

	}
}

bool	RequestHandler::checkResponseComplete(void) const
{
	return (_res_state == RES_FINISHED);
}

void	RequestHandler::processReqData(void)
{
	bool	keep_processing;

	keep_processing = true;
	while (keep_processing)
	{
		switch (_req_state)
		{
			case REQ_READING_HEADERS:
				if (_httpRequest.parseHeaders(_request_buff))
				{
					std::string uri = _httpRequest.getPath();
					const std::vector<Location>& bookshelf = _server.getLocations();
					_location = matchLocation(uri, bookshelf);
					if (!_location)
					{
						_handler_error_code = NOT_FOUND;
						_req_state = REQ_ERROR;
					}
					else if (!_location->isMethodAllowed(_httpRequest.getMethod()))
					{
						_handler_error_code = METHOD_NOT_ALLOWED;
						_req_state = REQ_ERROR;
					}
					else if (_httpRequest.hasBody() && _httpRequest.getContentLength() > _location->getClientMaxBodySize())
					{
							_handler_error_code = PAYLOAD_TOO_LARGE;
							_req_state = REQ_ERROR;
					}
					else
					{
						if (_httpRequest.hasBody())
							_req_state = REQ_READING_BODY;
						else
							_req_state = REQ_COMPLETE;
					}
				}
				else
				{
					if (_httpRequest.hasError())
						_req_state = REQ_ERROR;
					else
						keep_processing = false;
				}
				break;

			case REQ_READING_BODY:
				if (_httpRequest.parseBody(_request_buff))
					_req_state = REQ_COMPLETE;
				else
					if (_httpRequest.hasError())
						_req_state = REQ_ERROR;
					else
						keep_processing = false;
				break;

			case REQ_COMPLETE:
				keep_processing = false;
				break;

			case REQ_ERROR:
				if (_httpRequest.hasError() || _handler_error_code == PAYLOAD_TOO_LARGE)
				{
					_should_close_connection = true;
					_request_buff.clear();
				}
				keep_processing = false;
				break;
		}
	}
}

bool	RequestHandler::readFileContent(const std::string& file_path, std::string& out_content)
{
	std::ifstream file(file_path.c_str());
	if (!file.is_open())
	{
		return (false);
	}
	std::ostringstream ss;
	ss << file.rdbuf();
	out_content = ss.str();
	return (true);
}

std::string RequestHandler::buildDefaultErrorHtml(HttpStatus error_code)
{
	std::ostringstream ss;
	ss << "<html>\r\n";
	ss << "<head><title>Error " << error_code << "</title></head>\r\n";
	ss << "<body>\r\n";
	ss << "<h1>" << error_code << " " << _httpResponse.getReasonPhrase() << "</h1>\r\n";
	ss << "</body>\r\n";
	ss << "</html>\r\n";
	return (ss.str());
}

std::string RequestHandler::getErrorPagePath(HttpStatus error_code)
{
	const Location* target_location = _location;
	if (!target_location)
	{
		target_location = getDefaultLocation();
	}
	if (!target_location)
	{
		return ("");
	}
	const std::map<int, std::string>& error_pages = target_location->getErrorPages();
	std::map<int, std::string>::const_iterator it = error_pages.find(error_code);
	if (it == error_pages.end())
	{
		return ("");
	}
	std::string custom_path = it->second;
	std::string root = target_location->getRoot();
	if (!root.empty() && root[root.length() - 1] != '/' && !custom_path.empty() && custom_path[0] != '/')
	{
		root += "/";
	}
	std::string full_path = root + custom_path;
	struct stat file_stat;
	if (stat(full_path.c_str(), &file_stat) == 0)
	{
		if (S_ISREG(file_stat.st_mode))
		{
			return (full_path);
		}
	}
	return ("");
}

void    RequestHandler::buildResponseData(void)
{
	HttpStatus final_error = NONE;

	if (_httpRequest.hasError())
	{
		final_error = _httpRequest.getError();
	}
	else if (_handler_error_code == PAYLOAD_TOO_LARGE)
	{
		final_error = _handler_error_code;
	}
	if (!_httpRequest.wantsKeepAlive() ||
		_httpRequest.hasError() ||
		final_error == PAYLOAD_TOO_LARGE)
	{
		_should_close_connection = true;
	}
	if (final_error != NONE)
	{
		std::string body_content;
		std::string error_file_path = getErrorPagePath(final_error);
		if (!readFileContent(error_file_path, body_content))
		{
			body_content = buildDefaultErrorHtml(final_error);
		}
		_httpResponse.buildErrorPage(final_error, body_content);
	}
	else
	{
		_httpResponse.buildNormalResponse();
	}
	if (_httpRequest.hasError() || final_error == PAYLOAD_TOO_LARGE)
		_httpResponse.addHeader("Connection", "close");
}

const Location*	RequestHandler::matchLocation(const std::string& request_uri, const std::vector<Location>& location)
{
	for (size_t i = 0; i < location.size(); ++i)
	{
		if (request_uri.find(location[i].getPrefix()) == 0)
		{
			return &location[i];
		}
	}
	return (NULL);
}
