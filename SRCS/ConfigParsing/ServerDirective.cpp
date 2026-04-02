#include "ServerDirective.hpp"

ServerDirective::ServerDirective(void)
	:ADirectiveBlock("server", (s_directive_rules){HTTP, 1, 1, true}, SERVER)
{
}

ServerDirective::~ServerDirective(void)
{
}
