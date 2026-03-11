#ifndef EPOLL_HPP
# define EPOLL_HPP

# include <stdexcept>

class	Server;
class	Client;
class	ClientSocket;

class	Epoll
{
	public:
		Epoll(void);
		~Epoll(void);
	private:
		Epoll(const Epoll& other);
		Epoll&	operator=(const Epoll& other);

	public:
		void	addServerToPoll(Server& server);
		void	addClientToPoll(ClientSocket& client);
		void	runEvents(void);

	private:
		const int			_fd;
		static const int	_MAX_EVENTS;

	public:
		class	CreateException : public std::exception
		{
			const char*	what(void) const throw();
		};
};

#endif
