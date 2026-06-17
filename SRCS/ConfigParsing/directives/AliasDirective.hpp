#ifndef ALIASDIRECTIVE_HPP
# define ALIASDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	AliasDirective : public ADirective
{
	public:
		AliasDirective(void);
		~AliasDirective(void);
	private:
		AliasDirective(const AliasDirective& other);
		AliasDirective&	operator=(const AliasDirective& other);

	public:
		void	setConfig(Config& config) const;
};

#endif
