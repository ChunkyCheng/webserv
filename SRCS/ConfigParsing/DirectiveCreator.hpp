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
								   int block_level, std::vector<ADirective*>& others);
		
		typedef ADirective* (*t_creator)(void);
		typedef std::map<std::string, t_creator>	t_creator_map;
	
	private:
		static const t_creator_map	_creator_map;
		
		static t_creator_map	_create_map(void);
};

#endif
