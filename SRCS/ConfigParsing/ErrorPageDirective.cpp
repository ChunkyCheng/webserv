#include "ErrorPageDirective.hpp"
#include <cstdlib>
#include <climits>
#include <cerrno>
#include "ConfigExcept.hpp"

ErrorPageDirective::ErrorPageDirective(ConfigParser& info)
	:ADirective("error_page",
	(s_directive_rules){HTTP | SERVER | LOCATION, 3, INT_MAX, true}, info)
{
}

ErrorPageDirective::~ErrorPageDirective(void)
{
}

static bool	strisdigit(std::string& str)
{
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i]))
			return (false);
	}
	return (true);
}

void	ErrorPageDirective::parse(ConfigParser& info)
{
	long		errorcode;
	std::string	dest;

	(void)info;
	if (_argv.size() < 3)
		throw (ConfigExcept(ConfigExcept::WRONG_ARGC, _argv[0], _config_path));
	dest = _argv[_argv.size() - 1].value;
	for (unsigned int i = 1; i < _argv.size() - 1; ++i)
	{
		if (!strisdigit(_argv[i].value))
			throw (ConfigExcept(ConfigExcept::INVALID_VAL, _argv[i], _config_path));
		errorcode = std::strtol(_argv[i].value.c_str(), NULL, 10);
		if (errorcode < 300 || errorcode > 599 || errno == ERANGE)
			throw (ConfigExcept(ConfigExcept::ECODE_RANGE, _argv[i], _config_path));
		_code_destinations[errorcode] = dest;
	}
}
		

