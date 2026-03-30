#include "ADirective.hpp"
#include "ConfigParser.hpp"
#include <iostream>

ADirective::ADirective(std::string type, s_directive_rules rules, ConfigParser& info) 
	:_type(type),
	 _rules(rules),
	 _tokens(info.getTokens()),
	 _config_path(info.getConfigPath())
{
	while (_tokens.front().type != EOF_TOK && _tokens.front().type != SYMBOL)
	{
		_argv.push_back(_tokens.front());
		_tokens.pop_front();
	}
}

ADirective::~ADirective(void)
{
}

void	ADirective::perrorInvalidArgC(void) const
{
	std::cerr << "Error: invalid number of arguments in \""
			  << _type << "\" directive in "
			  << _config_path << ":" << _tokens[0].line_num << std::endl;
}

void	ADirective::perrorDuplicate(void) const
{
	std::cerr << "Error: \""
			  << _type << "\" directive is duplicate in "
			  << _config_path << ":" << _tokens[0].line_num << std::endl;
}
