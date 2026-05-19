#include "UploadDirective.hpp"

UploadDirective::UploadDirective(void)
	:ADirective("upload", UPLOAD_RULES)
{
}

UploadDirective::~UploadDirective(void)
{
}

void	UploadDirective::parse(t_tokens& tokens, const std::string& config_path)
{
	(void)tokens;
	if (_argv[1].value == "on")
		_on = true;
	else if (_argv[1].value == "off")
		_on = false;
	else
		throw (ConfigExcept(ConfigExcept::INVALID_BOOL, _argv[1], _type, config_path));
}

void	UploadDirective::setConfig(Config& config) const
{
	config.upload_allowed = _on;
}
