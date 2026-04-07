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

	private:
		void	checkConflict(ADirective* other, const std::string& config_path);
};

#endif
