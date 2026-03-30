#ifndef DIRECTIVEEXCEPTION_HPP
# define DIRECTIVEEXCEPTION_HPP

# include "ConfigParser.h"

class	DirectiveException : public std::exception
{
	public:
		const char* what(void) const throw();

	private:
		std::string	_config_path;
		std::string	_directive;
		std::string	_line_num;
		std::string	_message;
};

#endif
