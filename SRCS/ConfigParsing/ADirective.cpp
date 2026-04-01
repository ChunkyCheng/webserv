#include "ADirective.hpp"
#include "ConfigParser.hpp"
#include "ConfigExcept.hpp"
#include <iostream>

ADirective::ADirective(std::string type, s_directive_rules rules, ConfigParser& info) 
	:_type(type),
	 _rules(rules),
	 _tokens(info.getTokens()),
	 _config_path(info.getConfigPath())
{
}

ADirective::~ADirective(void)
{
}

void	ADirective::init(std::deque<s_token>& tokens, ConfigParser& info)
{
	while (tokens.front().type != EOF_TOK && tokens.front().type != SYMBOL)
	{
		_argv.push_back(tokens.front());
		tokens.pop_front();
	}
	consumeSymbolToken(tokens);
	parse(info);
}

void	ADirective::consumeSymbolToken(std::deque<s_token>& tokens)
{
	if (tokens.front().type == EOF_TOK)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_EOF, tokens.front(), _config_path));
	if (tokens.front().value != ";")
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_TOK, tokens.front(), _config_path));
	tokens.pop_front();
}

void	ADirective::parse(ConfigParser& info)
{
	(void)info;
}
