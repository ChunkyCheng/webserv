#include "ListenDirective.hpp"
#include "ConfigExcept.hpp"

ListenDirective::ListenDirective(void)
	:ADirective("listen", LISTEN_RULES)
{
}

ListenDirective::~ListenDirective(void)
{
}

void	ListenDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	(void)tokens;
	_host = _argv[1].value;
	if (false)
		throw (ConfigExcept(ConfigExcept::HOST_NOT_FOUND, _argv[1], config_path));
}
