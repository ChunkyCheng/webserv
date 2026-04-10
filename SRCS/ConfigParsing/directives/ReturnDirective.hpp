#ifndef RETURNDIRECTIVE_HPP
# define RETURNDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	ReturnDirective : public ADirective
{
	public:
		ReturnDirective(void);
		~ReturnDirective(void);
	private:
		ReturnDirective(const ReturnDirective& other);
		ReturnDirective&	operator=(const ReturnDirective& other);
	
	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	setConfig(Config& config) const;

	private:
		unsigned int	_code;
		std::string		_target;
};

#endif
