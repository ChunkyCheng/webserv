#include "LocationDirective.hpp"
#include "Location.hpp"

LocationDirective::LocationDirective(void)
	:ADirectiveBlock("location", LOCATION_RULES, LOCATION)
{
}

LocationDirective::~LocationDirective(void)
{
}

void	LocationDirective::checkConflict(ADirective* other, const std::string& config_path)
{
	if (_argv[1].value == dynamic_cast<LocationDirective*>(other)->_argv[1].value)
		throw (ConfigExcept(ConfigExcept::CONFLICT, _argv[0], config_path));
}

Location	LocationDirective::createLocation(Config config) const
{
	for (unsigned int i = 0; i < _subdirectives.size(); ++i)
		_subdirectives[i]->setConfig(config);
	return (Location(config));
}
