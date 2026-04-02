#ifndef LOCATIONDIRECTIVE_HPP
# define LOCATIONDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirectiveBlock.hpp"

class	LocationDirective : public ADirectiveBlock
{
	public:
		LocationDirective(void);
		~LocationDirective(void);
	private:
		LocationDirective(const LocationDirective& other);
		LocationDirective&	operator=(const LocationDirective& other);

	protected:
	private:
};

#endif
