#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Config.hpp"

class	Location
{
	public:
		Location(Config& config);
		Location(const Location& other);
		Location&	operator=(const Location& other);
		~Location(void);
	private:
		Location(void);

	private:
		Config	_config;
};

#endif
