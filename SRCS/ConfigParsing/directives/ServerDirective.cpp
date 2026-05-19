#include "ServerDirective.hpp"
#include "LocationDirective.hpp"
#include "Server.hpp"
#include <algorithm>

ServerDirective::ServerDirective(void)
	:ADirectiveBlock("server", SERVER_RULES, SERVER)
{
}

ServerDirective::~ServerDirective(void)
{
}

static bool	compare_location_prefix(Location& a, Location& b)
{
	return (a.getPrefix().length() > b.getPrefix().length());
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
	std::sort(locations.begin(), locations.end(), &compare_location_prefix);
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
