#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <fstream>
# include "ConfigParser.h"

class	WebServer;
class	Server;

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

		void					printTokens(void) const;
		void					parseTokens(void);
		std::vector<Server*>	createServers(WebServer& webserver) const;

	private:
		const std::string			_config_path;
		std::deque<s_token>			_tokens;
		int							_line_num;
		std::vector<ADirective*>	_directives;
		
		void	_tokenize(std::ifstream& infile);
};

#endif
