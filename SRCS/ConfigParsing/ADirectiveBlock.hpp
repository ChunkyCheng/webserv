#ifndef ADIRECTIVEBLOCK_HPP
# define ADIRECTIVEBLOCK_HPP

# include "ADirective.hpp"

class	ADirectiveBlock	: public ADirective
{
	protected:
		ADirectiveBlock(std::deque<s_token>& tokens, std::string type,
						const std::string& config_path, s_directive_rules rules,
						int block_level);
		~ADirectiveBlock(void);
	private:
		ADirectiveBlock(void);
		ADirectiveBlock(const ADirectiveBlock& other);
		ADirectiveBlock&	operator=(const ADirectiveBlock& other);

	public:
		void	parseSubDirectives(void);

	private:
		std::vector<ADirective*>	_sub_directives;
		int							_block_level;
};

#endif
