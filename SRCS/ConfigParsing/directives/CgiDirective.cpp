#include "CgiDirective.hpp"
#include "Config.hpp"

CgiDirective::CgiDirective(void)
	:ADirective("cgi", CGI_RULES)
{
}

CgiDirective::~CgiDirective(void)
{
}

void	CgiDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	(void)tokens;
	_extension = _argv[1].value;
	_exec_path = _argv[2].value;
	_script_dir = _argv[3].value;
	if (_extension.empty() || _extension[0] != '.')
		throw (ConfigExcept(ConfigExcept::INVALID_VAL, _argv[1], _type, config_path));
}

void	CgiDirective::checkConflict(ADirective* other, const std::string& config_path)
{
	if (_extension == dynamic_cast<CgiDirective*>(other)->_extension)
		throw (ConfigExcept(ConfigExcept::CONFLICT, _argv[1], _type, config_path));
}

void	CgiDirective::setConfig(Config& config) const
{
	Config::s_cgi_info	info;

	info.exec_path = _exec_path;
	info.script_dir = _script_dir;
	config.cgi_info[_extension] = info;
}