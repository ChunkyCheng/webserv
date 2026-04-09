#include "AutoindexDirective.hpp"

AutoindexDirective::AutoindexDirective(void)
	:ADirective("autoindex", AUTOINDEX_RULES)
{
}

AutoindexDirective::~AutoindexDirective(void)
{
}

void	AutoindexDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	(void)tokens;
	if (_argv[1].value == "on")
		_on = true;
	else if (_argv[1].value == "off")
		_on = false;
	else
		throw (ConfigExcept(ConfigExcept::INVALID_BOOL, _argv[1], _type, config_path));
}

void	AutoindexDirective::setConfig(Config& config) const
{
	config.autoindex = _on;
}
