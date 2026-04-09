#ifndef CONFIGEXCEPT_HPP
# define CONFIGEXCEPT_HPP

# include "ConfigParser.h"
# include <stdexcept>

class	ConfigExcept : public std::exception
{
	public:
		enum e_ecode
		{
			UNEXPECTED_TOK,
			UNEXPECTED_EOF,
			UNKNOWN_DIR,
			WRONG_SCOPE,
			NO_BLOCKOPEN,
			WRONG_ARGC,
			INVALID_VAL,
			ECODE_RANGE,
			HOST_NOT_FOUND,
			INVALID_METHOD,
			INVALID_BOOL,
			DUPLICATE,
			CONFLICT
		};
		ConfigExcept(e_ecode ecode, s_token token, std::string config_path);
		ConfigExcept(e_ecode ecode, s_token token, std::string directive, std::string config_path);
		~ConfigExcept(void) throw();

		const char*	what(void) const throw();

	private:
		std::string	_message;
};

#endif
