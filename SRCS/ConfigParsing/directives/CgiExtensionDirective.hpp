#ifndef CGIEXTENSIONDIRECTIVE_HPP
# define CGIEXTENSIONDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	CgiExtensionDirective : public ADirective
{
	public:
		CgiExtensionDirective(void);
		~CgiExtensionDirective(void);
	private:
		CgiExtensionDirective(const CgiExtensionDirective& other);
		CgiExtensionDirective&	operator=(const CgiExtensionDirective& other);

	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	checkConflict(ADirective* other, const std::string& config_path);
		void	setConfig(Config& config) const;

	private:
		std::string	_extension;
};

#endif