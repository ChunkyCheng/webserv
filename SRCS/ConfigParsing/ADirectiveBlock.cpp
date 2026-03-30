#include "ADirectiveBlock.hpp"
#include "ConfigExcept.hpp"
#include "DirectiveCreator.hpp"
#include "ConfigParser.hpp"

ADirectiveBlock::ADirectiveBlock(std::string type, s_directive_rules rules,
								 ConfigParser& info, int block_level)
	:ADirective(type, rules, info),
	 _block_level(block_level)
{
	if (_tokens.front().type == EOF_TOK || _tokens.front().value != "{")
		throw (ConfigExcept(ConfigExcept::NO_BLOCKOPEN, _argv[0], _config_path));
	_tokens.pop_front();
	parseSubDirectives(info);
}

ADirectiveBlock::~ADirectiveBlock(void)
{
	for (unsigned int i = 0; i < _sub_directives.size(); ++i)
		delete _sub_directives[i];
}

void	ADirectiveBlock::parseSubDirectives(ConfigParser& info)
{
	while (_tokens.front().type != EOF_TOK && _tokens.front().value != "}")
	{
		_sub_directives.push_back(DirectiveCreator::create(info, _block_level));
	}
	if (_tokens.front().type == EOF_TOK)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_EOF, _tokens.front(), _config_path));
	else
		_tokens.pop_front();
}
