#ifndef CGISTDOUTPIPE_HPP
# define CGISTDOUTPIPE_HPP

# include "ISocket.hpp"
# include "CGIHandler.hpp"
# include <sys/epoll.h>

class CGIHandler;

class CGIStdoutPipe: public ISocket
{
	public:
		CGIStdoutPipe(int fd, CGIHandler& parent);
		~CGIStdoutPipe(void);

		int		getFd(void) const;
		void	handleEvent(int events);
	
	private:
		CGIStdoutPipe(void);
		CGIStdoutPipe(const CGIStdoutPipe& other);
		CGIStdoutPipe&	operator=(const CGIStdoutPipe& other);

		int		_fd;
		CGIHandler&	_parent;
};

#endif