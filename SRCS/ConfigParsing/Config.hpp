#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ConfigParser.h"

class	Config
{
	public:
		Config(void);
		Config(const Config& other);
		Config&	operator=(const Config& other);
		~Config(void);

		struct s_client_max_body_size
		{
			bool		is_defined;
			long long	value;
		};
		struct s_return
		{
			int			code;
			std::string	target;
		};
		struct s_cgi_info
		{
			std::string	exec_path;
			std::string	script_dir;
		};

		s_client_max_body_size				client_max_body_size;
		s_return							return_info;
		std::vector<std::string>			index;
		bool								autoindex;
		std::string							root;
		std::map<int, std::string>			error_pages;
		bool								upload_allowed;
		std::string							upload_store;
		std::map<std::string, s_cgi_info>	cgi_info;
	
		//Location only	
		bool								method_get;
		bool								method_post;
		bool								method_delete;

		//Server only
		std::vector<std::string>			listens;
};

#endif
