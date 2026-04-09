#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Config.hpp"

class	Location
{
	public:
		Location(const std::string& prefix, Config& config);
		Location(const Location& other);
		Location&	operator=(const Location& other);
		~Location(void);
	private:
		Location(void);

	public:
		const std::string&	getPrefix(void) const;

	private:
		std::string	_prefix;
		Config		_config;
};

#endif
