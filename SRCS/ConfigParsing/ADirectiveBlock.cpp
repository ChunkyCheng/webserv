#include "ADirectiveBlock.hpp"
#include "ConfigExcept.hpp"
#include "DirectiveCreator.hpp"
#include "ConfigParser.hpp"

ADirectiveBlock::ADirectiveBlock(std::string type,
								 s_directive_rules rules,
								 int block_level)
	:ADirective(type, rules),
	 _block_level(block_level)
{
}

ADirectiveBlock::~ADirectiveBlock(void)
{
	for (unsigned int i = 0; i < _subdirectives.size(); ++i)
		delete _subdirectives[i];
}

void	ADirectiveBlock::consumeSymbolToken(t_tokens& tokens,
const std::string& config_path)
{
	if (tokens.front().type == EOF_TOK || tokens.front().value != "{")
		throw (ConfigExcept(ConfigExcept::NO_BLOCKOPEN, _argv[0], config_path));
	tokens.pop_front();
}

void	ADirectiveBlock::parse(t_tokens& tokens, const std::string& config_path)
{
	_parseSubDirectives(tokens, config_path);
}

void	ADirectiveBlock::_parseSubDirectives(t_tokens& tokens,
const std::string& config_path)
{
	while (tokens.front().type != EOF_TOK && tokens.front().value != "}")
	{
		_subdirectives.push_back
		(DirectiveCreator::create(tokens, config_path,_block_level));
	}
	if (tokens.front().type == EOF_TOK)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_EOF, tokens.front(), config_path));
	else
		tokens.pop_front();
}
