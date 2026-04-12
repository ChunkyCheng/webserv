#ifndef ERRORPAGEDIRECTIVE_HPP
# define ERRORPAGEDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	ErrorPageDirective : public ADirective
{
	public:
		ErrorPageDirective(void);
		~ErrorPageDirective(void);
	private:
		ErrorPageDirective(const ErrorPageDirective& other);
		ErrorPageDirective&	operator=(const ErrorPageDirective& other);

	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	setConfig(Config& config) const;

	private:
		std::map<int, std::string>	_code_destinations;
};

#endif
