#ifndef METHODSDIRECTIVE_HPP
# define METHODSDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	MethodsDirective : public ADirective
{
	public:
		MethodsDirective(void);
		~MethodsDirective(void);
	private:
		MethodsDirective(const MethodsDirective& other);
		MethodsDirective&	operator=(const MethodsDirective& other);

	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	setConfig(Config& config) const;

	private:
		std::map<std::string, bool>	_allowed;
};

#endif
