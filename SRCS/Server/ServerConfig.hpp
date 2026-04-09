#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

class	ServerConfig
{
	public:
		ServerConfig(void);
		ServerConfig(const ServerConfig& other);
		ServerConfig&	operator=(const ServerConfig& other);
		~ServerConfig(void);
		
		typedef struct s_LocationConfig
		{
			std::string	prefix;
			bool		methodGET;
			bool		methodPOST;
			bool		methodDELETE;
			int			redirection_code;
			std::string	redirection_url;
			std::string	root;
			std::string	index;
			bool		autoindex;
			bool		allow_upload;
			std::string	upload_store;
			std::string	cgi_extension;
			std::string	cgi_path;
		}	t_location;

		typedef struct s_config
		{	
			int									client_max_body_size;
			int									redirection_code;
			std::string							redirection_url;
			std::vector<std::string>			socket_addr;
			std::map<int, std::string>			error_pages;
			std::map<std::string, t_location>	location_config;
		}	t_Config;

	protected:
	private:
};

#endif
