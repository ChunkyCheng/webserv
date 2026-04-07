#include "DirectiveCreator.hpp"
#include "ConfigParser.hpp"
#include "ADirective.hpp"
#include "ConfigExcept.hpp"
#include "ServerDirective.hpp"
#include "LocationDirective.hpp"
#include "ErrorPageDirective.hpp"
#include "ListenDirective.hpp"
#include "MethodDirective.hpp"
#include <memory>

static ADirective*	createServer(void)
{
	return (new ServerDirective());
}

static ADirective*	createLocation(void)
{
	return (new LocationDirective());
}

static ADirective*	createErrorPage(void)
{
	return (new ErrorPageDirective());
}

static ADirective*	createListen(void)
{
	return (new ListenDirective());
}

static ADirective*	createMethod(void)
{
	return (new MethodDirective());
}

DirectiveCreator::t_creator_map	 DirectiveCreator::_create_map(void)
{
	t_creator_map	map;

	map["server"]		= &createServer;
	map["location"]		= &createLocation;
	map["error_page"]	= &createErrorPage;
	map["listen"]		= &createListen;
	map["method"]		= &createMethod;
	return (map);
}

const DirectiveCreator::t_creator_map DirectiveCreator::_creator_map = _create_map();

ADirective*	DirectiveCreator::create(t_tokens& tokens, const t_str& config_path,
int block_level, std::vector<ADirective*>& others)
{
	std::string					type;
	std::auto_ptr<ADirective>	directive;

	if (tokens.front().type != WORD)
		throw (ConfigExcept(ConfigExcept::UNEXPECTED_TOK, tokens.front(), config_path));
	type = tokens.front().value;
	if (!_creator_map.count(type))
		throw (ConfigExcept(ConfigExcept::UNKNOWN_DIR, tokens.front(), config_path));
	directive = std::auto_ptr<ADirective>(_creator_map.at(type)());
	directive->init(tokens, config_path, block_level, others);
	return (directive.release());
}
