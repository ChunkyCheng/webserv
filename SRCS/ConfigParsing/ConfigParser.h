#ifndef CONFIGPARSER_H
# define CONFIGPARSER_H

# include <iostream>
# include <string>
# include <deque>
# include <vector>
# include <map>

# define HTTP		0b001
# define SERVER		0b010
# define LOCATION	0b100

class ADirective;

enum e_token_type {WORD, SYMBOL};
struct s_token
{
	std::string		value;
	e_token_type	type;
	int				line_num;
};

struct s_directive_rules
{
	int				block_scope;
	unsigned int	min_args;
	unsigned int	max_args;
	bool			allow_multiple;
};

#endif
