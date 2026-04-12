#include "ConfigParser.hpp"
#include <iostream>
#include <cstdlib>
#include <climits>
#include "ADirective.hpp"
#include "DirectiveCreator.hpp"
#include "ServerDirective.hpp"
#include "WebServer.hpp"
#include "Server.hpp"

ConfigParser::ConfigParser(std::string file_path)
	:_config_path(file_path)
{
	std::ifstream	infile;

	infile.open(file_path.c_str());
	if (!infile.is_open())
	{
		std::cerr << "Error: Failed to open \"" << file_path << "\"" << std::endl;
		return ;
	}
	_tokenize(infile);
}

ConfigParser::~ConfigParser(void)
{
	for (unsigned int i = 0; i < _directives.size(); ++i)
		delete _directives[i];
}

const std::deque<s_token>&	ConfigParser::getTokens(void) const
{
	return (_tokens);
}

const std::string&	ConfigParser::getConfigPath(void) const
{
	return (_config_path);
}

static bool	issymbol(const char c)
{
	if (c == '{' || c == '}' || c == ';')
		return (true);
	return (false);
}

static std::string	getword(std::string& line, unsigned int start)
{
	unsigned int	end;

	end = start;
	while (end < line.length()
			&& line[end] != '#'
			&& !std::isspace(line[end])
			&& !issymbol(line[end]))
		++end;
	return (line.substr(start, end - start));
}

void	ConfigParser::_tokenize(std::ifstream& infile)
{
	int			line_num;
	std::string	line;

	line_num = 0;
	while (getline(infile, line))
	{
		++line_num;
		for (unsigned int i = 0; i < line.length(); ++i)
		{
			if (line[i] == '#')
				break ;
			if (std::isspace(line[i]))
				continue;
			else if (issymbol(line[i]))
				_tokens.push_back((s_token){std::string(line, i, 1), SYMBOL, line_num});
			else
			{
				_tokens.push_back((s_token){getword(line, i), WORD, line_num});
				i += _tokens.back().value.length() - 1;
			}
		}
	}
	_tokens.push_back((s_token){"", EOF_TOK, line_num});
}

void	ConfigParser::printTokens(void) const
{
	for (unsigned int i = 0; i < _tokens.size(); ++i)
	{
		if (i != 0)
			std::cout << " ";
		if (_tokens[i].type == WORD)
			std::cout << "\e[33m";
		else
			std::cout << "\e[31m";
		std::cout << _tokens[i].value;
	}
	std::cout << "\e[0m" << std::endl;
}

void	ConfigParser::parseTokens(void)
{
	while (_tokens.front().type != EOF_TOK)
	{
		_directives.push_back
		(DirectiveCreator::create(_tokens, _config_path, HTTP, _directives));
	}
}

std::vector<Server*>	ConfigParser::createServers(WebServer& webserver) const
{

	std::vector<ServerDirective*>	server_directives;
	std::vector<Server*>			servers;
	Config							config;

	for (unsigned int i = 0; i < _directives.size(); ++i)
	{
		if (_directives[i]->getType() == "server")
			server_directives.push_back(dynamic_cast<ServerDirective*>(_directives[i]));
		else
			_directives[i]->setConfig(config);
	}
	for (unsigned int i = 0; i < server_directives.size(); ++i)
	{
		servers.push_back(server_directives[i]->createServer(webserver, config));
		if (servers.back() == NULL)
			servers.pop_back();
	}
	return (servers);
}
