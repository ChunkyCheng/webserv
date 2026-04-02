#ifndef ADIRECTIVE_HPP
# define ADIRECTIVE_HPP

# include "ConfigParser.h"

class	ADirective
{
	protected:
		ADirective(std::string type, s_directive_rules rules);
	public:
		virtual ~ADirective(void);
	private:
		ADirective(void);
		ADirective(const ADirective& other);
		ADirective&	operator=(const ADirective& other);

	public:
		void	init(t_tokens& tokens, const std::string& config_path);

	protected:
		const std::string		_type;
		const s_directive_rules	_rules;
		std::vector<s_token>	_argv;

		virtual void	consumeSymbolToken(t_tokens& tokens,
										   const std::string& config_path);
		virtual void	parse(t_tokens& tokens, const std::string& config_path);

	private:
};

#endif
