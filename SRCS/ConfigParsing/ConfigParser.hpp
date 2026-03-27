#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <deque>
# include <map>
# include <fstream>
# include <string>
# include "ConfigParser.h"

typedef struct s_rules
{
	int		block_scope;
	int		min_argc;
	int		max_argc;
}	s_rules;

class	ConfigParser
{
	public:
		ConfigParser(std::string file_path);
		~ConfigParser(void);
	private:
		ConfigParser(void);
		ConfigParser(const ConfigParser& other);
		ConfigParser&	operator=(const ConfigParser& other);

	public:
		void	printTokens(void) const;
		bool	parseTokens(void);

	private:
		std::string					_config_path;
		std::deque<s_token>			_tokens;
		std::vector<ADirective*>	_directives;
		
		static const std::map<std::string, s_rules>	_directive_rules;
		
		void	_tokenize(std::ifstream& infile);
};

#endif
