#include "CGIStdoutPipe.hpp"

CGIStdoutPipe::CGIStdoutPipe(int fd, CGIHandler& parent):
	_fd(fd), _parent(parent)
{}

CGIStdoutPipe::~CGIStdoutPipe(void) {}

int		CGIStdoutPipe::getFd(void) const
{
	return _fd;
}

void	CGIStdoutPipe::handleEvent(int events)
{
	if (events & EPOLLIN)
		_parent.onStdoutReady();
	if (events & (EPOLLHUP | EPOLLERR))
		_parent.onStdoutHup();
}