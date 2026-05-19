#include "RootDirective.hpp"
#include "Config.hpp"

RootDirective::RootDirective(void)
	:ADirective("root", ROOT_RULES)
{
}

RootDirective::~RootDirective(void)
{
}

void	RootDirective::setConfig(Config& config) const
{
	config.root = _argv[1].value;
}
