#include "CgiExtensionDirective.hpp"

CgiExtensionDirective::CgiExtensionDirective(void)
	:ADirective("cgi_extension", CGI_EXTENSION_RULES)
{
}

CgiExtensionDirective::~CgiExtensionDirective(void)
{
}

void	CgiExtensionDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	(void)tokens;
	_extension = _argv[1].value;
	if (_extension.empty() || _extension[0] != '.')
		throw (ConfigExcept(ConfigExcept::INVALID_VAL, _argv[1], _type, config_path));
}

void	CgiExtensionDirective::checkConflict(ADirective* other, const std::string& config_path)
{
	if (_extension == dynamic_cast<CgiExtensionDirective*>(other)->_extension)
		throw (ConfigExcept(ConfigExcept::CONFLICT, _argv[1], _type, config_path));
}

void	CgiExtensionDirective::setConfig(Config& config) const
{
	Config::s_cgi_info	info;

	info.exec_path = config.cgi_default_exec_path;
	info.script_dir = config.root;
	config.cgi_info[_extension] = info;
}