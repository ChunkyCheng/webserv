#ifndef ADIRECTIVEBLOCK_HPP
# define ADIRECTIVEBLOCK_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	ADirectiveBlock	: public ADirective
{
	protected:
		ADirectiveBlock(std::string type, s_directive_rules rules, ConfigParser& info,
						int block_level);
		~ADirectiveBlock(void);
	private:
		ADirectiveBlock(void);
		ADirectiveBlock(const ADirectiveBlock& other);
		ADirectiveBlock&	operator=(const ADirectiveBlock& other);

	public:
		void	parseSubDirectives(ConfigParser& info);

	private:
		std::vector<ADirective*>	_sub_directives;
		int							_block_level;
};

#endif
