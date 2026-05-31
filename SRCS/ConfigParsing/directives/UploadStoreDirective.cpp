#include "UploadStoreDirective.hpp"
#include "Config.hpp"

UploadStoreDirective::UploadStoreDirective(void)
	:ADirective("upload_store", UPLOAD_STORE_RULES)
{
}

UploadStoreDirective::~UploadStoreDirective(void)
{
}

void	UploadStoreDirective::setConfig(Config& config) const
{
	config.upload_store = _argv[1].value;
}
