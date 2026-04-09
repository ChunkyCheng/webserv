#include "ErrorPageDirective.hpp"
#include <cstdlib>
#include <climits>
#include <cerrno>
#include "ConfigExcept.hpp"

ErrorPageDirective::ErrorPageDirective(void)
	:ADirective("error_page", ERROR_PAGE_RULES)
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

void	ErrorPageDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	long		errorcode;
	std::string	dest;

	(void)tokens;
	dest = _argv[_argv.size() - 1].value;
	for (unsigned int i = 1; i < _argv.size() - 1; ++i)
	{
		if (!strisdigit(_argv[i].value))
			throw (ConfigExcept(ConfigExcept::INVALID_VAL, _argv[i], config_path));
		errorcode = std::strtol(_argv[i].value.c_str(), NULL, 10);
		if (errorcode < 300 || errorcode > 599 || errno == ERANGE)
			throw (ConfigExcept(ConfigExcept::ECODE_RANGE, _argv[i], config_path));
		_code_destinations[errorcode] = dest;
	}
}
