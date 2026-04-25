#include "ListenDirective.hpp"
#include "ConfigExcept.hpp"
#include <cstdlib>

ListenDirective::ListenDirective(void)
	:ADirective("listen", LISTEN_RULES)
{
}

ListenDirective::~ListenDirective(void)
{
}

void	ListenDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	std::string	port_str;
	int			port_int;

	(void)tokens;
	_host = _argv[1].value;
	if (_host.find(':') == std::string::npos)
	{
		port_str = _host;
		_host = "0.0.0.0:" + _host;
	}
	else
		port_str = _host.substr(_host.find(':') + 1);
	for (unsigned int i = 0; i < port_str.length(); ++i)
	{
		if (!std::isdigit(port_str[i]))
			throw (ConfigExcept(ConfigExcept::HOST_NOT_FOUND, _argv[1], config_path));
	}
	port_int = std::atoi(port_str.c_str());
	if (port_int < 1 || port_int > 65535 || port_str.length() > 5)
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
