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

		 

	private:
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

		s_client_max_body_size				_client_max_body_size;
		s_return							_return;
		std::vector<std::string>			_index;
		bool								_autoindex;
		std::string							_root;
		std::map<int, std::string>			_error_pages;
		bool								_upload_allowed;
		std::string							_upload_store;
		std::map<std::string, s_cgi_info>	_cgi_info;
};

#endif
