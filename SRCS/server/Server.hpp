#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>

class	Server
{
	public:
		Server(void);
		Server(const Server& other);
		Server&	operator=(const Server& other);
		~Server(void);

	protected:
	private:
		std::string	_port = "0.0.0.0:80";
		int			_listening_socket;
};

#endif
