#include "ADirective.hpp"
#include <iostream>

ADirective::ADirective(std::deque<s_token>& tokens, std::string type,
					   const std::string& config_path, s_directive_rules rules)
	:_tokens(tokens),
	 _type(type),
	 _config_path(config_path),
	 _rules(rules),
	 _valid(true)
{
	while (_tokens.size() && _tokens.front().type != SYMBOL)
	{
		_argv.push_back(_tokens.front());
		_tokens.pop_front();
	}
}

ADirective::~ADirective(void)
{
}

bool	ADirective::isValid(void) const
{
	return (_valid);
}

void	ADirective::perrorNotAllowedHere(void) const
{
	std::cerr << "Error: \""
			  << _type << "\" directive is not allowed here in "
			  << _config_path << ":" << _tokens[0].line_num << std::endl;
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
