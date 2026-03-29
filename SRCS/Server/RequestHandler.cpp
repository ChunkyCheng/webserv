#include "RequestHandler.hpp"
#include <cstdlib>
#include "../../httpProtocol/Utils.hpp"

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

	header_end = _request_buff.find("\r\n\r\n");
	// Check if header finished loading, if returns 0, return false
	if (header_end == std::string::npos)
		return (false);
	// Only return true if found the header
	// Find the method, POST usually have body
	if (_request_buff.compare(0, 4, "POST") == 0)
	{
		size_t pos = _request_buff.find("Content-Length: ");
		// Check there is Content-Length
		if (pos != std::string::npos && pos < header_end)
		{
			// Find the number between content length header, and the end \r\n, every header line follows this format
			size_t val_start = pos + 16;
			size_t val_end = _request_buff.find("\r\n", val_start);

			std::string	raw_val = _request_buff.substr(val_start, val_end - val_start);
			std::string clean_val = utils_trim(raw_val);
			long content_length = std::atol(clean_val.c_str());
			// It is possible the same client send another request immediately, so the current body
			// size will be longer than the content length intended, it accounted for the second's request too
			// the most important thing is that it finished fetching the first request, so return true first
			size_t total_header_size = header_end + 4;
			size_t current_body_size = _request_buff.size() - total_header_size;
				return (current_body_size >= (size_t)content_length);
		}

		// It is possible there is no Content-length, but there is body? 
		// Might be chunked?
		if (_request_buff.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			// Usually chunked request ends with 0\r\n\r\n
			return (_request_buff.find("0\r\n\r\n") != std::string::npos);
		}
	}
	// PROBLEM: What if body is not found? This logic immediately returns true
	// Needed reading: Transfer-encoding: chunked
	return (true);
}

void	RequestHandler::continueBuildResponse(void)
{
}

bool	RequestHandler::checkResponseComplete(void) const
{
	return (true);
}
