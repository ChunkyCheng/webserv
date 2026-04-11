#include "Location.hpp"
#include "utils.hpp"

Location::Location(const std::string& prefix, Config& config)
	:_prefix(prefix), _config(config)
{
}

Location::Location(const Location& other)
	:_prefix(other._prefix), _config(other._config)
{
}

Location&	Location::operator=(const Location& other)
{
	if (this != &other)
	{
		_prefix = other._prefix;
		_config = other._config;
	}
	return (*this);
}

Location::~Location(void)
{
}

const std::string&	Location::getPrefix(void) const
{
	return (_prefix);
}

const int&	Location::getReturnCode(void) const
{
	return (_config.return_info.code);
}

const std::string&	Location::getReturnTarget(void) const
{
	return (_config.return_info.target);
}

const bool&	Location::isGet(void) const
{
	return (_config.method_get);
}

const bool&	Location::isPost(void) const
{
	return (_config.method_post);
}

const bool&	Location::isDelete(void) const
{
	return (_config.method_delete);
}

const unsigned long long&	Location::getClientMaxBodySize(void) const
{
	return (_config.client_max_body_size);
}

const std::string&	Location::getRoot(void) const
{
	return (_config.root);
}

const std::vector<std::string>&	Location::getIndex(void) const
{
	return (_config.index);
}

const bool&	Location::isAutoindex(void) const
{
	return (_config.autoindex);
}

const std::map<int, std::string>&	Location::getErrorPages(void) const
{
	return (_config.error_pages);
}

std::ostream&	operator<<(std::ostream& os, const Location& obj)
{
	os << "Prefix: " << obj.getPrefix() << "\n";
	os << "Return: " << obj.getReturnCode() << " " << obj.getReturnTarget() << "\n";
	os << "Methods:";
	if (obj.isGet())
		os << " GET";
	if (obj.isPost())
		os << " POST";
	if (obj.isDelete())
		os << " DELETE";
	if (!obj.isGet() && !obj.isPost() && !obj.isDelete())
		os << " NONE";
	os << "\n";
	os << "Client max body size: " << obj.getClientMaxBodySize() << "\n";
	os << "Root: " << obj.getRoot() << "\n";
	os << "Index: " << obj.getIndex() << "\n";
	os << "Autoindex: ";
	if (obj.isAutoindex())
		os << "on" << "\n";
	else
		os << "off" << "\n";
	os << "Error pages: " << obj.getErrorPages();
	return (os);
}
