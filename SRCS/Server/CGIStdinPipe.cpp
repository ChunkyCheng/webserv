#include "CGIStdinPipe.hpp"
#include <sys/epoll.h>

CGIStdinPipe::CGIStdinPipe(int fd, CGIHandler& parent)
	: _fd(fd), _parent(parent)
{}

CGIStdinPipe::~CGIStdinPipe(void) {}

int		CGIStdinPipe::getFd(void) const
{
	return _fd;
}

void	CGIStdinPipe::handleEvent(int events)
{
	if (events & EPOLLOUT)
		_parent.onStdinReady();
	if (events & (EPOLLHUP | EPOLLERR))
		_parent.onStdinReady();
}