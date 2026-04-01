#ifndef ERRORPAGEDIRECTIVE_HPP
# define ERRORPAGEDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	ErrorPageDirective : public ADirective
{
	public:
		ErrorPageDirective(ConfigParser& info);
		~ErrorPageDirective(void);
	private:
		ErrorPageDirective(void);
		ErrorPageDirective(const ErrorPageDirective& other);
		ErrorPageDirective&	operator=(const ErrorPageDirective& other);

	public:
		void	parse(ConfigParser& info);

	private:
		std::map<int, std::string>	_code_destinations;
};

#endif
