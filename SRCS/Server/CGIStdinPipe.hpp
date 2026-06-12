#ifndef CGISTDINPIPE_HPP
# define CGISTDINPIPE_HPP

class CGIHandler;

class CGIStdinPipe: public Isocket
{
	public:
		CGIStdinPipe(int fd, CGIHandler& parent);
		~CGIStdinPipe(void);

		int		getFd(void) const;
		void	handleEvent(int events);
	
	private:
		CGIStdinPipe(void);
		CGIStdinPipe(const CGIStdinPipe& other);
		CGIStdinPipe&	operator=(const CGIStdinPipe& other);

		int		_fd;
		CGIHandler&	_parent;
}

#endif