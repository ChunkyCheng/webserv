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
	if (_host.find(':') == std::string::npos)
		_host = "0.0.0.0:" + _host;
	if (false)
		throw (ConfigExcept(ConfigExcept::HOST_NOT_FOUND, _argv[1], config_path));
}

void	ListenDirective::checkConflict(ADirective* other, const std::string& config_path)
{
	if (_host == dynamic_cast<ListenDirective*>(other)->_host)
		throw (ConfigExcept(ConfigExcept::CONFLICT, _argv[0], _type, config_path));
}

void	ListenDirective::setConfig(Config& config) const
{
	config.listens.push_back(_host);
}
