#ifndef ADIRECTIVEBLOCK_HPP
# define ADIRECTIVEBLOCK_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	ADirectiveBlock	: public ADirective
{
	protected:
		ADirectiveBlock(std::string type, s_rules rules, int block_level);
		~ADirectiveBlock(void);
	private:
		ADirectiveBlock(void);
		ADirectiveBlock(const ADirectiveBlock& other);
		ADirectiveBlock&	operator=(const ADirectiveBlock& other);

	protected:
		void	consumeSymbolToken(t_tokens& tokens, const std::string& config_path);
		void	parse(t_tokens& tokens, const std::string& config_path);

	private:
		std::vector<ADirective*>	_subdirectives;
		int							_block_level;

		void	_parseSubDirectives(t_tokens& tokens, const std::string& config_path);
};

#endif
