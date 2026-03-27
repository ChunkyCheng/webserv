#include "ServerDirective.hpp"

ServerDirective::ServerDirective(std::deque<s_token>& tokens,
const std::string& config_path)
	:ADirectiveBlock(tokens, "server", config_path, (s_directive_rules){HTTP, 0, 0, true},SERVER)
{
}

ServerDirective::~ServerDirective(void)
{
}
