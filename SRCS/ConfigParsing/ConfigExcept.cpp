#include "ConfigExcept.hpp"
#include <sstream>

ConfigExcept::ConfigExcept(e_ecode ecode, s_token token, std::string config_path)
{
	std::stringstream	ss;

	ss << "Error: ";
	switch (ecode)
	{
		case UNEXPECTED_TOK:
			ss << "unexpected \"" << token.value << "\"";
			break ;
		case UNEXPECTED_EOF:
			ss << "unexpected end of file, expecting \"}\"";
			break ;
		case UNKNOWN_DIR:
			ss << "unknown directive \"" << token.value << "\"";
			break ;
		case SCOPE:
			ss << "\"" << token.value << "\" directive is not allowed here";
			break ;
		case NO_BLOCKOPEN:
			ss << "directive \"" << token.value << "\" has no opening \"{\"";
			break ;
		default:
			break ;
	}
	ss << " in " << config_path << ":" << token.line_num;
	_message = ss.str();
}

ConfigExcept::~ConfigExcept(void) throw()
{
}

const char*	ConfigExcept::what(void) const throw()
{
	return (_message.c_str());
}
