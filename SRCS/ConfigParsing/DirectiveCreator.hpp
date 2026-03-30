#ifndef DIRECTIVECREATOR_HPP
# define DIRECTIVECREATOR_HPP

# include "ConfigParser.h"

class	DirectiveCreator
{
	private:
		DirectiveCreator(void);
		DirectiveCreator(const DirectiveCreator& other);
		DirectiveCreator&	operator=(const DirectiveCreator& other);
		~DirectiveCreator(void);
	
	public:
		static ADirective*	create(ConfigParser& info, int block_level);
		
		typedef ADirective* (*t_creator)(ConfigParser& info);
		struct s_create_info
		{
			unsigned int	block_scope;
			unsigned int	min_argc;
			unsigned int	max_argc;
			t_creator		creator;
		};
	
	private:
		static const std::map<std::string, s_create_info>	_creator_map;
		
		static std::map<std::string, DirectiveCreator::s_create_info> _create_map(void);
};

#endif
