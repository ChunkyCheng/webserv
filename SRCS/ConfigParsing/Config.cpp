#include "Config.hpp"

Config::Config(void)
	:_client_max_body_size((s_client_max_body_size){false, 1 << 20}),
	 _autoindex(false),
	 _upload_allowed(false)
{
}

Config::Config(const Config& other)
	:_client_max_body_size(other._client_max_body_size),
	 _return(other._return),
	 _index(other._index),
	 _autoindex(other._autoindex),
	 _root(other._root),
	 _error_pages(other._error_pages),
	 _upload_allowed(other._upload_allowed),
	 _upload_store(other._upload_store),
	 _cgi_info(other._cgi_info)
{
}

Config&	Config::operator=(const Config& other)
{
	if (this != &other)
	{
		_client_max_body_size = other._client_max_body_size;
		_return = other._return;
		_index = other._index;
		_autoindex = other._autoindex;
		_root = other._root;
		 _error_pages = other._error_pages;
		_upload_allowed = other._upload_allowed;
		_upload_store = other._upload_store;
		_cgi_info = other._cgi_info;
	}
	return (*this);
}

Config::~Config(void)
{
}
