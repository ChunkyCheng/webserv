#ifndef CGIPATHDIRECTIVE_HPP
# define CGIPATHDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	CgiPathDirective : public ADirective
{
	public:
		CgiPathDirective(void);
		~CgiPathDirective(void);
	private:
		CgiPathDirective(const CgiPathDirective& other);
		CgiPathDirective&	operator=(const CgiPathDirective& other);

	public:
		void	setConfig(Config& config) const;
};

#endif