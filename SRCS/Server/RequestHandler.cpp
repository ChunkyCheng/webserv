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
	return (false);
}

void	RequestHandler::continueBuildResponse(void)
{
}

bool	RequestHandler::checkResponseComplete(void) const
{
	return (true);
}
