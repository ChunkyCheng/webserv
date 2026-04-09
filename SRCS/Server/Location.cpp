#include "Location.hpp"

Location::Location(const std::string& prefix, Config& config)
	:_prefix(prefix), _config(config)
{
}

Location::Location(const Location& other)
	:_prefix(other._prefix), _config(other._config)
{
}

Location&	Location::operator=(const Location& other)
{
	if (this != &other)
	{
		_prefix = other._prefix;
		_config = other._config;
	}
	return (*this);
}

Location::~Location(void)
{
}

const std::string&	Location::getPrefix(void) const
{
	return (_prefix);
}
