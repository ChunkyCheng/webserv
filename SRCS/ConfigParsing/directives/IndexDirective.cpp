#include "IndexDirective.hpp"

IndexDirective::IndexDirective(void)
	:ADirective("index", INDEX_RULES)
{
}

IndexDirective::~IndexDirective(void)
{
}

void	IndexDirective::setConfig(Config& config) const
{
	std::vector<std::string>	files;

	for (unsigned int i = 1; i < _argv.size(); ++i)
		files.push_back(_argv[i].value);
	config.index = files;
}
