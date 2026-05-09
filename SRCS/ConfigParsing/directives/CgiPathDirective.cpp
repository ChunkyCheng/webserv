#include "CgiPathDirective.hpp"

CgiPathDirective::CgiPathDirective(void)
	:ADirective("cgi_path", CGI_PATH_RULES)
{
}

CgiPathDirective::~CgiPathDirective(void)
{
}

void	CgiPathDirective::setConfig(Config& config) const
{
	std::map<std::string, Config::s_cgi_info>::iterator	it;

	config.cgi_default_exec_path = _argv[1].value;
	for (it = config.cgi_info.begin(); it != config.cgi_info.end(); ++it)
	{
		it->second.exec_path = _argv[1].value;
	}
}