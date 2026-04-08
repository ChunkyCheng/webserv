#include "ServerDirective.hpp"
#include "LocationDirective.hpp"
#include "Server.hpp"

ServerDirective::ServerDirective(void)
	:ADirectiveBlock("server", SERVER_RULES, SERVER)
{
}

ServerDirective::~ServerDirective(void)
{
}

Server*	ServerDirective::createServer(WebServer& webserver, Config config) const
{
	std::vector<LocationDirective*>	location_directives;
	std::vector<Location> 			locations;

	for (unsigned int i = 0; i < _subdirectives.size(); ++i)
	{
		if (_subdirectives[i]->getType() == "location")
			location_directives.push_back(dynamic_cast<LocationDirective*>(_subdirectives[i]));
		else
			_subdirectives[i]->setConfig(config);
	}
	for (unsigned int i = 0; i < location_directives.size(); ++i)
		locations.push_back(location_directives[i]->createLocation(config));
	try
	{
		return (new Server(webserver, locations, config));
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (NULL);
	}
}
