#ifndef METHODDIRECTIVE_HPP
# define METHODDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	MethodDirective : public ADirective
{
	public:
		MethodDirective(void);
		~MethodDirective(void);
	private:
		MethodDirective(const MethodDirective& other);
		MethodDirective&	operator=(const MethodDirective& other);

	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	setConfig(Config& config) const;

	private:
		std::map<std::string, bool>	_allowed;
};

#endif
