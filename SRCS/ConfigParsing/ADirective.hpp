#ifndef ADIRECTIVE_HPP
# define ADIRECTIVE_HPP

# include "ConfigParser.h"

class	ADirective
{
	protected:
		ADirective(std::deque<s_token>& tokens, std::string type,
				   const std::string& config_path, s_directive_rules rules);
	public:
		virtual ~ADirective(void);
	private:
		ADirective(void);
		ADirective(const ADirective& other);
		ADirective&	operator=(const ADirective& other);
	
	public:
		bool	isValid(void) const;

	protected:
		void	perrorNotAllowedHere(void) const;
		void	perrorInvalidArgC(void) const;
		void	perrorDuplicate(void) const;

		std::deque<s_token>&	_tokens;
		const std::string		_type;
		const std::string		_config_path;
		const s_directive_rules	_rules;
		std::vector<s_token>	_argv;
		bool					_valid;

	private:
};

#endif
