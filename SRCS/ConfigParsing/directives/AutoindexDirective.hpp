#ifndef AUTOINDEXDIRECTIVE_HPP
# define AUTOINDEXDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	AutoindexDirective : public ADirective
{
	public:
		AutoindexDirective(void);
		~AutoindexDirective(void);
	private:
		AutoindexDirective(const AutoindexDirective& other);
		AutoindexDirective&	operator=(const AutoindexDirective& other);

	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	setConfig(Config& config) const;
	
	private:
		bool	_on;
};

#endif
