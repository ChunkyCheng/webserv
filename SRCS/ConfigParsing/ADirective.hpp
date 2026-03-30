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

	protected:
		void	perrorNotAllowedHere(void) const;
		void	perrorInvalidArgC(void) const;
		void	perrorDuplicate(void) const;

		const std::string		_type;
		const s_directive_rules	_rules;
		std::deque<s_token>&	_tokens;
		const std::string&		_config_path;
		std::vector<s_token>	_argv;

	private:
};

#endif
