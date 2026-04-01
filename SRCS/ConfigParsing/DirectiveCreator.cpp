#include "DirectiveCreator.hpp"
#include "ConfigParser.hpp"
#include "ADirective.hpp"
#include "ConfigExcept.hpp"
#include "ServerDirective.hpp"
#include "LocationDirective.hpp"
#include "ErrorPageDirective.hpp"
#include <memory>

static ADirective*	createServer(ConfigParser& info)
{
	return (new ServerDirective(info));
}

static ADirective*	createLocation(ConfigParser& info)
{
	return (new LocationDirective(info));
}

static ADirective* createErrorPage(ConfigParser& info)
{
	return (new ErrorPageDirective(info));
}

std::map<std::string, DirectiveCreator::s_create_info> DirectiveCreator::_create_map(void)
{
	std::map<std::string, DirectiveCreator::s_create_info> map;

	map["server"] = (s_create_info){HTTP, 1, 1, &createServer};
	map["location"] = (s_create_info){SERVER, 2, 2, &createLocation};
	map["error_page"] = (s_create_info){HTTP | SERVER | LOCATION, 3, 10, &createErrorPage};
	return (map);
}

const std::map<std::string, DirectiveCreator::s_create_info>
DirectiveCreator::_creator_map = _create_map();

ADirective*	DirectiveCreator::create(ConfigParser& info, int block_level)
{
	std::deque<s_token>&	tokens = info.getTokens();
	const std::string&		config_path = info.getConfigPath();

	std::string					type;
	std::auto_ptr<ADirective>	directive;

	if (tokens.front().type != WORD)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_TOK, tokens.front(), config_path));
	type = tokens.front().value;
	if (!_creator_map.count(type))
		throw (ConfigExcept(ConfigExcept::UNKNOWN_DIR, tokens.front(), config_path));
	if (!(_creator_map.at(type).block_scope & block_level))
		throw (ConfigExcept(ConfigExcept::WRONG_SCOPE, tokens.front(), config_path));
	directive = std::auto_ptr<ADirective>(_creator_map.at(type).creator(info));
	directive->init(tokens, info);
	return (directive.release());
}
