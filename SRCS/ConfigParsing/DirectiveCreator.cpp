#include "DirectiveCreator.hpp"
#include "ADirective.hpp"
#include "ServerDirective.hpp"

static ADirective*	createServer(std::deque<s_token>& tokens, const std::string& config_path)
{
	return (new ServerDirective(tokens, config_path));
}

std::map<std::string, DirectiveCreator::s_create_info> DirectiveCreator::_create_map(void)
{
	std::map<std::string, DirectiveCreator::s_create_info> map;

	map["server"] = (s_create_info){HTTP, 1, 1, &createServer};
	return (map);
}

const std::map<std::string, DirectiveCreator::s_create_info>
DirectiveCreator::_creator_map = _create_map();

ADirective*	DirectiveCreator::create(std::deque<s_token>& tokens,
const std::string& config_path, int block_level)
{
	std::string	type;
	int			line_num;
	ADirective*	newDirective;

	if (tokens.front().type != WORD)
	{
		std::cerr << "Error: unexpected \"" << tokens.front().value
				  << "\" in " << config_path
				  << ":" << tokens.front().line_num << std::endl;
		return (NULL);
	}
	type = tokens.front().value;
	line_num = tokens.front().line_num;
	if (!_creator_map.count(type))
	{
		std::cerr << "Error: unknown directive \"" << type
				  << "\" in " << config_path
				  << ":" << line_num << std::endl;
		return (NULL);
	}
	if (!(_creator_map.at(type).block_scope & block_level))
	{
		std::cerr << "naur scope" << std::endl;
		return (NULL);
	}
	newDirective = _creator_map.at(type).creator(tokens, config_path);
	if (!newDirective->isValid())
	{
		delete newDirective;
		return (NULL);
	}
	return (newDirective);
}
