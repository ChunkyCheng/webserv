#ifndef ADIRECTIVE_HPP
# define ADIRECTIVE_HPP

# include "ConfigParser.h"

class	ADirective
{
	public:
		struct s_rules
		{
			unsigned int	block_scope;
			unsigned int	min_argc;
			unsigned int	max_argc;
			bool			allow_multiple;
		};
	protected:
		ADirective(std::string type, s_rules rules);
	public:
		virtual ~ADirective(void);
	private:
		ADirective(void);
		ADirective(const ADirective& other);
		ADirective&	operator=(const ADirective& other);

	public:
		void	init(t_tokens& tokens, const t_str& config_path, int block_level);

	protected:
		const std::string		_type;
		const s_rules			_rules;
		std::vector<s_token>	_argv;

		virtual void	consumeSymbolToken(t_tokens& tokens, const t_str& config_path);
		virtual void	parse(t_tokens& tokens, const t_str& config_path);
};

#endif
