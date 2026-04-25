#ifndef EPOLL_HPP
# define EPOLL_HPP

# include <stdexcept>

class	ISocket;

class	Epoll
{
	public:
		Epoll(void);
		~Epoll(void);
	private:
		Epoll(const Epoll& other);
		Epoll&	operator=(const Epoll& other);

	public:
		void	addSocketToPoll(ISocket& socket);
		void	modAddSendEvent(ISocket& socket);
		void	modRemoveSendEvent(ISocket& socket);
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
