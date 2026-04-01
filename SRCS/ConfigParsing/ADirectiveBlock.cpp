#include "ADirectiveBlock.hpp"
#include "ConfigExcept.hpp"
#include "DirectiveCreator.hpp"
#include "ConfigParser.hpp"

ADirectiveBlock::ADirectiveBlock(std::string type, s_directive_rules rules,
								 ConfigParser& info, int block_level)
	:ADirective(type, rules, info),
	 _block_level(block_level)
{
}

ADirectiveBlock::~ADirectiveBlock(void)
{
	_freeSubDirectives();
}

void	ADirectiveBlock::consumeSymbolToken(std::deque<s_token>& tokens)
{
	std::cout << _tokens.size() << std::endl;
	if (tokens.front().type == EOF_TOK || tokens.front().value != "{")
		throw (ConfigExcept(ConfigExcept::NO_BLOCKOPEN, _argv[0], _config_path));
	_tokens.pop_front();
}

void	ADirectiveBlock::parse(ConfigParser& info)
{
	_parseSubDirectives(info);
}

void	ADirectiveBlock::_parseSubDirectives(ConfigParser& info)
{
	while (_tokens.front().type != EOF_TOK && _tokens.front().value != "}")
		_subdirectives.push_back(DirectiveCreator::create(info, _block_level));
	if (_tokens.front().type == EOF_TOK)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_EOF, _tokens.front(), _config_path));
	else
		_tokens.pop_front();
}

void	ADirectiveBlock::_freeSubDirectives(void)
{
	for (unsigned int i = 0; i < _subdirectives.size(); ++i)
		delete _subdirectives[i];
}


