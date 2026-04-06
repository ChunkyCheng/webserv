#include "LocationDirective.hpp"

LocationDirective::LocationDirective(void)
	:ADirectiveBlock("location", (s_directive_rules){SERVER, 2, 2, true}, LOCATION)
{
}

LocationDirective::~LocationDirective(void)
{
}
