#include "AliasDirective.hpp"
#include "Config.hpp"

AliasDirective::AliasDirective(void)
	:ADirective("alias", ROOT_RULES)
{
}

AliasDirective::~AliasDirective(void)
{
}

void	AliasDirective::setConfig(Config& config) const
{
	config.root = "";
	config.alias = _argv[1].value;
}
