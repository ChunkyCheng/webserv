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
		static ADirective*	create(t_tokens& tokens, const std::string& config_path,
								   int block_level);
		
		typedef ADirective* (*t_creator)(void);
	
	private:
		static const std::map<std::string, t_creator>	_creator_map;
		
		static std::map<std::string, t_creator> _create_map(void);
};

#endif
