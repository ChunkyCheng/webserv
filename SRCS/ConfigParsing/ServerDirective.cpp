#include "ServerDirective.hpp"
#include "ConfigParser.hpp"

ServerDirective::ServerDirective(ConfigParser& info)
	:ADirectiveBlock("server", (s_directive_rules){HTTP, 0, 0, true}, info, SERVER)
{
}

ServerDirective::~ServerDirective(void)
{
}
