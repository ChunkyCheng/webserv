#include "DirectiveCreator.hpp"
#include "ConfigParser.hpp"
#include "ADirective.hpp"
#include "ConfigExcept.hpp"
#include "ServerDirective.hpp"

static ADirective*	createServer(ConfigParser& info)
{
	return (new ServerDirective(info));
}

std::map<std::string, DirectiveCreator::s_create_info> DirectiveCreator::_create_map(void)
{
	std::map<std::string, DirectiveCreator::s_create_info> map;

	map["server"] = (s_create_info){HTTP, 1, 1, &createServer};
	return (map);
}

const std::map<std::string, DirectiveCreator::s_create_info>
DirectiveCreator::_creator_map = _create_map();

ADirective*	DirectiveCreator::create(ConfigParser& info, int block_level)
{
	std::deque<s_token> tokens = info.getTokens();
	const std::string&	config_path = info.getConfigPath();

	std::string	type;
	ADirective*	newDirective;

	if (tokens.front().type != WORD)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_TOK, tokens.front(), config_path));
	type = tokens.front().value;
	if (!_creator_map.count(type))
		throw (ConfigExcept(ConfigExcept::UNKNOWN_DIR, tokens.front(), config_path));
	if (!(_creator_map.at(type).block_scope & block_level))
		throw (ConfigExcept(ConfigExcept::SCOPE, tokens.front(), config_path));
	newDirective = _creator_map.at(type).creator(info);
	return (newDirective);
}
