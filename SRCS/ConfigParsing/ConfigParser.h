#ifndef CONFIGPARSER_H
# define CONFIGPARSER_H

# include <iostream>
# include <string>
# include <deque>
# include <vector>
# include <map>
# include <stdexcept>
# include <climits>

# define HTTP		0b001
# define SERVER		0b010
# define LOCATION	0b100
# define ALL_BLOCK	0b111

# define CLIENT_MAX_BODY_RULES	(ADirective::s_rules){ALL_BLOCK, 2, 2, false}
# define ROOT_RULES				(ADirective::s_rules){ALL_BLOCK, 2, 2, false}
# define ERROR_PAGE_RULES		(ADirective::s_rules){ALL_BLOCK, 2, INT_MAX, true}
# define UPLOAD_ALLOWED_RULES	(ADirective::s_rules){ALL_BLOCK, 1, 1, false}
# define UPLOAD_STORE_RULES		(ADirective::s_rules){ALL_BLOCK, 1, 1, false}
# define INDEX_RULES			(ADirective::s_rules){ALL_BLOCK, 2, INT_MAX, false}
# define AUTOINDEX_RULES		(ADirective::s_rules){ALL_BLOCK, 2, 2, false}
# define CGI_RULES				(ADirective::s_rules){ALL_BLOCK, 4, 4, false}
# define SERVER_RULES			(ADirective::s_rules){HTTP, 1, 1, true}
# define LOCATION_RULES			(ADirective::s_rules){SERVER, 2, 2, true}
# define LISTEN_RULES			(ADirective::s_rules){SERVER, 2, 2, true}
# define METHOD_RULES			(ADirective::s_rules){LOCATION, 2, 4, false}
# define RETURN_RULES			(ADirective::s_rules){LOCATION, 2, 3, false}

class ConfigParser;
class ADirective;
class Config;

enum e_token_type {WORD, SYMBOL, EOF_TOK};
struct s_token
{
	std::string		value;
	e_token_type	type;
	int				line_num;
};
typedef std::deque<s_token>	t_tokens;
typedef std::string			t_str;

#endif
