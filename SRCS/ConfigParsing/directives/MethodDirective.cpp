#include "MethodDirective.hpp"

MethodDirective::MethodDirective(void)
	:ADirective("method", METHOD_RULES)
{
	_allowed["GET"] = false;
	_allowed["POST"] = false;
	_allowed["DELETE"] = false;
}

MethodDirective::~MethodDirective(void)
{
}

void	MethodDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	(void)tokens;
	for (unsigned int i = 1; i < _argv.size(); ++i)
	{
		if (!_allowed.count(_argv[i].value))
			throw (ConfigExcept(ConfigExcept::INVALID_METHOD, _argv[i], config_path));
		_allowed[_argv[i].value] = true;
	}
}

void	MethodDirective::setConfig(Config& config) const
{
	config.method_get = _allowed.at("GET");
	config.method_post = _allowed.at("POST");
	config.method_delete = _allowed.at("DELETE");
}
