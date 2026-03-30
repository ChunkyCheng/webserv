#ifndef SERVERDIRECTIVE_HPP
# define SERVERDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirectiveBlock.hpp"

class	ServerDirective : public ADirectiveBlock
{
	public:
		ServerDirective(ConfigParser& info);
		~ServerDirective(void);
	private:
		ServerDirective(void);
		ServerDirective(const ServerDirective& other);
		ServerDirective&	operator=(const ServerDirective& other);

	protected:
	private:
};

#endif
