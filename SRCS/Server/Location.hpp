#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "socket.h"
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
		const std::string&					getPrefix(void) const;
		const unsigned int&					getReturnCode(void) const;
		const std::string&					getReturnTarget(void) const;
		const bool&							isGet(void) const;
		const bool&							isPost(void) const;
		const bool&							isDelete(void) const;
		const unsigned long long&			getClientMaxBodySize(void) const;
		const std::string&					getRoot(void) const;
		const std::vector<std::string>&		getIndex(void) const;
		const bool&							isAutoindex(void) const;
		const std::map<int, std::string>&	getErrorPages(void) const;

	private:
		std::string	_prefix;
		Config		_config;
};

#endif
