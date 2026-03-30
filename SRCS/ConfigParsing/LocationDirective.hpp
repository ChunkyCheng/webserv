#ifndef LOCATIONDIRECTIVE_HPP
# define LOCATIONDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirectiveBlock.hpp"

class	LocationDirective : public ADirectiveBlock
{
	public:
		LocationDirective(ConfigParser& info);
		~LocationDirective(void);
	private:
		LocationDirective(void);
		LocationDirective(const LocationDirective& other);
		LocationDirective&	operator=(const LocationDirective& other);

	protected:
	private:
};

#endif
