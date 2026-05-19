#ifndef CLIENTMAXDIRECTIVE_HPP
# define CLIENTMAXDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	ClientMaxDirective : public ADirective
{
	public:
		ClientMaxDirective(void);
		~ClientMaxDirective(void);
	private:
		ClientMaxDirective(const ClientMaxDirective& other);
		ClientMaxDirective&	operator=(const ClientMaxDirective& other);
	
	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	setConfig(Config& config) const;

	protected:
	private:
		unsigned long long	_size;
};

#endif
