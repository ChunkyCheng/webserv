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
		case WRONG_SCOPE:
			ss << "\"" << token.value << "\" directive is not allowed here";
			break ;
		case NO_BLOCKOPEN:
			ss << "directive \"" << token.value << "\" has no opening \"{\"";
			break ;
		case WRONG_ARGC:
			ss << "invalid number of arguments in \"" << token.value << "\" directive";
			break ;
		case ECODE_RANGE:
			ss << "value \"" << token.value << "\" must be between 300 and 599";
			break ;
		case HOST_NOT_FOUND:
			ss << "host not found in \"" << token.value << "\" in \"listen\" directive";
			break ;
		case INVALID_METHOD:
			ss << "invalid method \"" << token.value << "\"";
			break ;
		case DUPLICATE:
			ss << "\"" << token.value << "\" directive is duplicate";
			break ;
		case CONFLICT:
			ss << "conflictig \"" << token.value << "\" directive";
			break ;
		default:
			break ;
	}
	ss << " in " << config_path << ":" << token.line_num;
	_message = ss.str();
}

ConfigExcept::ConfigExcept(e_ecode ecode, s_token token,
std::string directive, std::string config_path)
{
	std::stringstream	ss;

	ss << "Error: ";
	switch (ecode)
	{
		case CONFLICT:
			ss << "duplicate " << directive << " \"" << token.value << "\"";
			break ;
		case INVALID_VAL:
			ss << "invalid value \"" << token.value << "\" in " << directive;
			ss << " directive";
			break ;
		case INVALID_BOOL:
			ss << "invalid value \"" << token.value << "\" in " << directive;
			ss << " directive, it must be \"on\" or \"off\"";
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
