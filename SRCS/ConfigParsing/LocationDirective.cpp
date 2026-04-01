#include "LocationDirective.hpp"

LocationDirective::LocationDirective(ConfigParser& info)
	:ADirectiveBlock("location", (s_directive_rules){SERVER, 2, 2, true}, info, LOCATION)
{
}

LocationDirective::~LocationDirective(void)
{
}
