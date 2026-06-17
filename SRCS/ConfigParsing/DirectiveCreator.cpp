#include "DirectiveCreator.hpp"
#include "ConfigParser.hpp"
#include "ADirective.hpp"
#include "ConfigExcept.hpp"

#include "ServerDirective.hpp"
#include "LocationDirective.hpp"
#include "ErrorPageDirective.hpp"
#include "ListenDirective.hpp"
#include "ReturnDirective.hpp"
#include "MethodsDirective.hpp"
#include "ClientMaxDirective.hpp"
#include "RootDirective.hpp"
#include "AliasDirective.hpp"
#include "IndexDirective.hpp"
#include "AutoindexDirective.hpp"
#include "UploadDirective.hpp"
#include "UploadStoreDirective.hpp"
#include "CgiDirective.hpp"

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

static ADirective*	createReturn(void)
{
	return (new ReturnDirective());
}

static ADirective*	createMethods(void)
{
	return (new MethodsDirective());
}

static ADirective* createClientMax(void)
{
	return (new ClientMaxDirective());
}

static ADirective*	createRoot(void)
{
	return (new RootDirective());
}

static ADirective*	createAlias(void)
{
	return (new AliasDirective());
}

static ADirective*	createIndex(void)
{
	return (new IndexDirective());
}

static ADirective*	createAutoindex(void)
{
	return (new AutoindexDirective());
}

static ADirective* createUpload(void)
{
	return (new UploadDirective());
}

static ADirective* createUploadStore(void)
{
	return (new UploadStoreDirective());
}

static ADirective* createCgi(void)
{
	return (new CgiDirective());
}

DirectiveCreator::t_creator_map	 DirectiveCreator::_create_map(void)
{
	t_creator_map	map;

	map["server"]				= &createServer;
	map["location"]				= &createLocation;
	map["error_page"]			= &createErrorPage;
	map["listen"]				= &createListen;
	map["return"]				= &createReturn;
	map["methods"]				= &createMethods;
	map["client_max_body_size"]	= &createClientMax;
	map["root"]					= &createRoot;
	map["alias"]				= &createAlias;
	map["index"]				= &createIndex;
	map["autoindex"]			= &createAutoindex;
	map["upload"]				= &createUpload;
	map["upload_store"]			= &createUploadStore;
	map["cgi"]					= &createCgi;
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
