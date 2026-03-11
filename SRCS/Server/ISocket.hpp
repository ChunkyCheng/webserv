#ifndef ISOCKET_HPP
# define ISOCKET_HPP

# include <cerrno>
# include <cstring>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>


class	ISocket
{
	protected:
		ISocket(void) {};
		virtual ~ISocket(void) {};
	private:
		ISocket(const ISocket& other);
		ISocket&	operator=(const ISocket& other);
	
	public:
		virtual void	handleEvent(void) = 0;
};

#endif
