#include "CgiDirective.hpp"

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
	_executable = _argv[2].value;
	if (_extension[0] != '.' || _extension.length() < 2)
		throw (ConfigExcept(ConfigExcept::INVALID_EXTEN, _argv[1], config_path)); 
}

void	CgiDirective::checkConflict(ADirective* other, const std::string& config_path)
{
	if (_extension == dynamic_cast<CgiDirective*>(other)->_extension)
		throw (ConfigExcept(ConfigExcept::CONFLICT, _argv[0], _type, config_path));
}

void	CgiDirective::setConfig(Config& config) const
{
	config.cgi_map[_extension] = _executable;
}