#ifndef ADIRECTIVE_HPP
# define ADIRECTIVE_HPP

# include "ConfigParser.h"

class	ADirective
{
	protected:
		ADirective(std::string type, s_directive_rules rules, ConfigParser& info);
	public:
		virtual ~ADirective(void);
	private:
		ADirective(void);
		ADirective(const ADirective& other);
		ADirective&	operator=(const ADirective& other);

	public:
		void	init(std::deque<s_token>& tokens, ConfigParser& info);	

	protected:
		const std::string		_type;
		const s_directive_rules	_rules;
		std::deque<s_token>&	_tokens;
		const std::string&		_config_path;
		std::vector<s_token>	_argv;

		virtual void	consumeSymbolToken(std::deque<s_token>& tokens);
		virtual void	parse(ConfigParser& info);

	private:
};

#endif
