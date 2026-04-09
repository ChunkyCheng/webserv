#include "Config.hpp"

Config::Config(void)
	:client_max_body_size(1 << 20),
	 root("/var/www/html"),
	 autoindex(false),
	 upload_allowed(false)
{
}

Config::Config(const Config& other)
	:client_max_body_size(other.client_max_body_size),
	 return_info(other.return_info),
	 root(other.root),
	 index(other.index),
	 autoindex(other.autoindex),
	 error_pages(other.error_pages),
	 upload_allowed(other.upload_allowed),
	 upload_store(other.upload_store),
	 cgi_info(other.cgi_info),
	 method_get(other.method_get),
	 method_post(other.method_post),
	 method_delete(other.method_delete),
	 listens(other.listens)
{
}

Config&	Config::operator=(const Config& other)
{
	if (this != &other)
	{
		client_max_body_size = other.client_max_body_size;
		return_info = other.return_info;
		index = other.index;
		autoindex = other.autoindex;
		root = other.root;
		error_pages = other.error_pages;
		upload_allowed = other.upload_allowed;
		upload_store = other.upload_store;
		cgi_info = other.cgi_info;
		method_get = other.method_get;
		method_post = other.method_post;
		method_delete = other.method_delete;
		listens = other.listens;
	}
	return (*this);
}

Config::~Config(void)
{
}
