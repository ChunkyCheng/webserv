#include "RequestHandler.hpp"
#include <cstdlib>

RequestHandler::RequestHandler(Server& server,
std::string& req_buff, std::string& res_buff)
	: _server(server),
	_request_buff(req_buff),
	_response_buff(res_buff),
	_httpRequest(),
	_state(STATE_READING_HEADERS)
{
}

RequestHandler::~RequestHandler(void)
{
}

bool	RequestHandler::checkRequestComplete(void) const
{
	return (_state == STATE_COMPLETE || _state == STATE_ERROR);
}

void	RequestHandler::continueBuildResponse(void)
{
}

bool	RequestHandler::checkResponseComplete(void) const
{
	return (true);
}

void	RequestHandler::processData(void)
{
	bool	keep_processing;
	keep_processing = true;
	while (keep_processing)
	{
		switch (_state)
		{
			case STATE_READING_HEADERS:
				if (_httpRequest.parseHeaders(_request_buff))
				{
					_state = _httpRequest.hasBody() ? STATE_READING_BODY : STATE_COMPLETE;
				}
				else
				{
					keep_processing = false;
					if (_httpRequest.hasError())
						_state = STATE_ERROR;
				}
				break;

			case STATE_READING_BODY:
				if (_httpRequest.parseBody(_request_buff))
					_state = STATE_COMPLETE;
				else
					keep_processing = false;
				break;

			case STATE_COMPLETE:
				keep_processing = false;
				break;

			case STATE_ERROR:
				keep_processing = false;
				break;
		}
	}
}
