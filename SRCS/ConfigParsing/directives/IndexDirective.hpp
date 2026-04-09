#ifndef INDEXDIRECTIVE_HPP
# define INDEXDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	IndexDirective : public ADirective
{
	public:
		IndexDirective(void);
		~IndexDirective(void);
	private:
		IndexDirective(const IndexDirective& other);
		IndexDirective&	operator=(const IndexDirective& other);
	
	public:
		void	setConfig(Config& config) const;
};

#endif
