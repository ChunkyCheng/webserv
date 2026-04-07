#include "RequestHandler.hpp"
#include <cstdlib>

RequestHandler::RequestHandler(Server& server,
std::string& req_buff, std::string& res_buff)
	: _server(server),
	_request_buff(req_buff),
	_response_buff(res_buff),
	_httpRequest(),
	_req_state(REQ_READING_HEADERS),
	_res_state(RES_HEADERS)
{
}

RequestHandler::~RequestHandler(void)
{
}

bool	RequestHandler::checkRequestComplete(void) const
{
	return (_req_state == REQ_COMPLETE || _req_state == REQ_ERROR);
}

void	RequestHandler::continueBuildResponse(void)
{

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
				// if (isFatalError(_httpRequest.getError())) If is error, clear off buffer?
				// {
				// 	_request_buff.clear();
				// }
				keep_processing = false;
				break;
		}
	}
}

void	RequestHandler::buildResponseData(void)
{
	_httpResponse.prepareResponse();
}