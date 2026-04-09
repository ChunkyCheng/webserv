#ifndef ROOTDIRECTIVE_HPP
# define ROOTDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	RootDirective : public ADirective
{
	public:
		RootDirective(void);
		~RootDirective(void);
	private:
		RootDirective(const RootDirective& other);
		RootDirective&	operator=(const RootDirective& other);

	public:
		void	setConfig(Config& config) const;
};

#endif
