#include "RequestHandler.hpp"

RequestHandler::RequestHandler(Server& server,
std::string& req_buff, std::string& res_buff)
	:_server(server), _request_buff(req_buff), _response_buff(res_buff)
{
}

RequestHandler::~RequestHandler(void)
{
}

bool	RequestHandler::checkRequestComplete(void) const
{
	size_t	header_end;

	header_end = _request_buff.find("/r/n/r/n");
	// Check if header finished loading, if returns 0, return false
	if (header_end == std::string::npos)
		return (false);
	// Only return true if found the header
	// Find the method, POST usually have body
	if (_request_buff.compare(0, 4, "POST") == 0)
	{
		size_t pos = _request_buff.find("Content-Length: ");
		
	}
	
	// If we didn't find content-length, maybe it's "chunked?"
	if (_request_buff.find("Transfer-Encoding: chunked") != )
	// For GET, DELETE, once /r/n/r/n found, usually means done so return true to signal the start of response processing
	return (true);

	// PROBLEM: What if body is not found? This logic immediately returns true
	// Needed reading: Transfer-encoding: chunked
}

void	RequestHandler::continueBuildResponse(void)
{
}

bool	RequestHandler::checkResponseComplete(void) const
{
	return (true);
}
