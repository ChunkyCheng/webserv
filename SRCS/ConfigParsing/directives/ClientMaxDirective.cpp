#include "ClientMaxDirective.hpp"
#include <cstdlib>
#include <cerrno>

ClientMaxDirective::ClientMaxDirective(void)
	:ADirective("client_max_body_size", CLIENT_MAX_BODY_RULES)
{
}

ClientMaxDirective::~ClientMaxDirective(void)
{
}

void	ClientMaxDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	char				*endptr;
	unsigned long long	num;
	unsigned long long	multiplier;

	(void)tokens;
	num = std::strtoll(_argv[1].value.c_str(), &endptr, 10);
	if (*endptr == 'G')
		multiplier = 1 << 30;
	else if (*endptr == 'M')
		multiplier = 1 << 20;
	else if (*endptr == '\0')
		multiplier = 1;
	else
		throw (ConfigExcept(ConfigExcept::INVALID_VAL, _argv[1], _type, config_path));
	_size = num * multiplier;
	if (errno == ERANGE || errno == EINVAL || _size / multiplier != num)
		throw (ConfigExcept(ConfigExcept::INVALID_VAL, _argv[1], _type, config_path));
}

void	ClientMaxDirective::setConfig(Config& config) const
{
	config.client_max_body_size = _size;
}
