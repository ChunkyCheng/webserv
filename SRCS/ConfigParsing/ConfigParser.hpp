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
		std::deque<s_token>&	getTokens(void);
		const std::string&		getConfigPath(void) const;

		void	printTokens(void) const;
		bool	parseTokens(void);

	private:
		const std::string			_config_path;
		std::deque<s_token>			_tokens;
		int							_line_num;
		std::vector<ADirective*>	_directives;
		
		static const std::map<std::string, s_rules>	_directive_rules;
		
		void	_tokenize(std::ifstream& infile);
};

#endif
