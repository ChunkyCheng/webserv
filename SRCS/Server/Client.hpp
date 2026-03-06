#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <sys/epoll.h>

class	Client
{
	public:
		Client(void);
		Client(const Client& other);
		Client&	operator=(const Client& other);
		~Client(void);

	protected:
	private:
		const int	_socket;
};

#endif
