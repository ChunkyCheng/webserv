#include "Location.hpp"

Location::Location(Config& config)
	:_config(config)
{
}

Location::Location(const Location& other)
	:_config(other._config)
{
}

Location&	Location::operator=(const Location& other)
{
	if (this != &other)
	{
		_config = other._config;
	}
	return (*this);
}

Location::~Location(void)
{
}
