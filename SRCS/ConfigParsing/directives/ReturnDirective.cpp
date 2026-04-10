#include "ReturnDirective.hpp"
#include <cstdlib>

ReturnDirective::ReturnDirective(void)
	:ADirective("return", RETURN_RULES)
{
}

ReturnDirective::~ReturnDirective(void)
{
}

void	ReturnDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	(void)tokens;
	if (_argv[1].value.length() > 3)
		throw (ConfigExcept(ConfigExcept::INVALID_VAL, _argv[1], _type, config_path));
	for (unsigned int i = 0; i < _argv[1].value.length(); ++i)
	{
		if (std::isdigit(_argv[1].value[i]))
			throw (ConfigExcept(ConfigExcept::INVALID_VAL, _argv[1], _type, config_path));
	}
	_code = std::atoi(_argv[1].value.c_str());
	_target = _argv[2].value;
}

void	ReturnDirective::setConfig(Config& config) const
{
	config.return_info.code = _code;
	config.return_info.target = _target;
}
