#include "ADirectiveBlock.hpp"
#include "DirectiveCreator.hpp"

ADirectiveBlock::ADirectiveBlock(std::deque<s_token>& tokens, std::string type,
								 const std::string& config_path, s_directive_rules rules,
								 int block_level)
	:ADirective(tokens, type, config_path, rules), _block_level(block_level)
{
	if (_tokens.size() == 0 || _tokens.front().value != "{")
	{
		_valid = false;
		std::cerr << "Error: directive \"" << _type
				  << "\" has no opening \"{\" in " << _config_path
				  << ":" << _argv.back().line_num << std::endl;
		return ;
	}
	_tokens.pop_front();
	parseSubDirectives();
}

ADirectiveBlock::~ADirectiveBlock(void)
{
	for (unsigned int i = 0; i < _sub_directives.size(); ++i)
		delete _sub_directives[i];
}

void	ADirectiveBlock::parseSubDirectives(void)
{
	while (_tokens.size() && _tokens.front().value != "}")
	{
		_sub_directives.push_back(
		DirectiveCreator::create(_tokens, _config_path, _block_level));
		if (_sub_directives.back() == NULL || !_sub_directives.back()->isValid())
		{
			_valid = false;
			return ;
		}
	}
	if (_tokens.size() == 0)
	{
		_valid = false;
		std::cerr << "Error: unexpected end of file, expecting \"}\" in "
				  << _config_path << std::endl;
	}
	else
		_tokens.pop_front();
}
