#include "RequestHandler.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>

RequestHandler::RequestHandler(Server& server,
std::string& req_buff, std::string& res_buff)
	: _server(server),
	_request_buff(req_buff),
	_response_buff(res_buff),
	_httpRequest(),
	_req_state(REQ_READING_HEADERS),
	_res_state(RES_HEADERS),
	_matchedLocation(NULL),
	_isFileOpen(false),
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
	if (_fileInStream.is_open())
		_fileInStream.close();
}

bool	RequestHandler::checkRequestComplete(void) const
{
	return (_req_state == REQ_COMPLETE || _req_state == REQ_ERROR);
}

void	RequestHandler::continueBuildResponse(void)
{
	if (_res_state == RES_BODY)
	{
		if (_isFileOpen)
		{
			char chunk[8192];
			_fileInStream.read(chunk, sizeof(chunk));
			std::streamsize bytesRead = _fileInStream.gcount();
			if (bytesRead > 0)
			{
				_response_buff.append(chunk, bytesRead);
			}
			if (_fileInStream.eof() || _fileInStream.fail())
			{
				_fileInStream.close();
				_isFileOpen = false;
				_res_state = RES_FINISHED;
			}
		}
	}
	else
	{
		_response_buff += _httpResponse.getBody();
		_res_state = RES_FINISHED;
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
					_req_state = _httpRequest.hasBody() ? REQ_READING_BODY : REQ_COMPLETE;
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
				if (_httpRequest.hasFatalError())
				{
					_should_close_connection = true;
					_request_buff.clear();
				}
				else if (!_httpRequest.wantsKeepAlive())
				{
					_should_close_connection = true;
				}
				keep_processing = false;
				break;
		}
	}
}


void    RequestHandler::buildResponseData(void)
{
	if (_httpRequest.hasFatalError())
	{
		int error_code = _httpRequest.getError();
		buildFatalError(error_code);
	}
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
