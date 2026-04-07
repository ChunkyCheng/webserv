#ifndef LISTENDIRECTIVE_HPP
# define LISTENDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	ListenDirective : public ADirective
{
	public:
		ListenDirective(void);
		~ListenDirective(void);
	private:
		ListenDirective(const ListenDirective& other);
		ListenDirective&	operator=(const ListenDirective& other);

	public:
		void	parse(t_tokens& token, const std::string& config_path);

	private:
		std::string	_host;
};

#endif
