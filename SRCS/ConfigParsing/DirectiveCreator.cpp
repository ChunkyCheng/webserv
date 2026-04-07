#include "DirectiveCreator.hpp"
#include "ConfigParser.hpp"
#include "ADirective.hpp"
#include "ConfigExcept.hpp"
#include "ServerDirective.hpp"
#include "LocationDirective.hpp"
#include "ErrorPageDirective.hpp"
#include <memory>

static ADirective*	createServer(void)
{
	return (new ServerDirective());
}

static ADirective*	createLocation(void)
{
	return (new LocationDirective());
}

static ADirective* createErrorPage(void)
{
	return (new ErrorPageDirective());
}

std::map<std::string, DirectiveCreator::t_creator> DirectiveCreator::_create_map(void)
{
	std::map<std::string, DirectiveCreator::t_creator> map;

	map["server"] = &createServer;
	map["location"] = &createLocation;
	map["error_page"] = &createErrorPage;
	return (map);
}

const std::map<std::string, DirectiveCreator::t_creator>
DirectiveCreator::_creator_map = _create_map();

ADirective*	DirectiveCreator::create(t_tokens& tokens, const t_str& config_path, int block_level)
{
	std::string					type;
	std::auto_ptr<ADirective>	directive;

	if (tokens.front().type != WORD)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_TOK, tokens.front(), config_path));
	type = tokens.front().value;
	if (!_creator_map.count(type))
		throw (ConfigExcept(ConfigExcept::UNKNOWN_DIR, tokens.front(), config_path));
	directive = std::auto_ptr<ADirective>(_creator_map.at(type)());
	directive->init(tokens, config_path, block_level);
	return (directive.release());
}
