#include "ServerDirective.hpp"

ServerDirective::ServerDirective(ConfigParser& info)
	:ADirectiveBlock("server", (s_directive_rules){HTTP, 1, 1, true}, info, SERVER)
{
}

ServerDirective::~ServerDirective(void)
{
}
