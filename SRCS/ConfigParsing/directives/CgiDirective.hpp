#ifndef CGIDIRECTIVE_HPP
# define CGIDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	CgiDirective : public ADirective
{
	public:
		CgiDirective(void);
		~CgiDirective(void);
	private:
		CgiDirective(const CgiDirective& other);
		CgiDirective&	operator=(const CgiDirective& other);

	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	checkConflict(ADirective* other, const std::string& config_path);
		void	setConfig(Config& config) const;

	private:
		std::string	_extension;
		std::string	_exec_path;
		std::string	_script_dir;
};

#endif