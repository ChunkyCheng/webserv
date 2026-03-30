#include "DirectiveException.hpp"
#include <sstream>

DirectiveException::DirectiveException(std::string config_path, int line_num)
const char*	DirectiveException::what(void) const throw()
{
	return (_message.c_str());
}
