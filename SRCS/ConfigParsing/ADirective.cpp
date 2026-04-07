#include "ADirective.hpp"
#include "ConfigParser.hpp"
#include "ConfigExcept.hpp"
#include <iostream>

ADirective::ADirective(std::string type, s_rules rules) 
	:_type(type), _rules(rules)
{
}

ADirective::~ADirective(void)
{
}

void	ADirective::init(t_tokens& tokens, const std::string& config_path,
int block_level)
{
	if (!(block_level & _rules.block_scope))
		throw (ConfigExcept(ConfigExcept::WRONG_SCOPE, tokens.front(), config_path));
	while (tokens.front().type != EOF_TOK && tokens.front().type != SYMBOL)
	{
		_argv.push_back(tokens.front());
		tokens.pop_front();
	}
	consumeSymbolToken(tokens, config_path);
	if (_argv.size() < _rules.min_argc || _argv.size() > _rules.max_argc)
		throw (ConfigExcept(ConfigExcept::WRONG_ARGC, _argv[0], config_path));
	parse(tokens, config_path);
}

void	ADirective::consumeSymbolToken(t_tokens& tokens, const std::string& config_path)
{
	if (tokens.front().type == EOF_TOK)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_EOF, tokens.front(), config_path));
	if (tokens.front().value != ";")
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_TOK, tokens.front(), config_path));
	tokens.pop_front();
}

void	ADirective::parse(t_tokens& tokens, const std::string& config_path)
{
	(void)tokens;
	(void)config_path;
}
