#include "Epoll.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

const int	Epoll::_MAX_EVENTS = 128;

Epoll::Epoll(void)
	:_fd(epoll_create(1))
{
	if (_fd == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		throw (CreateException());
	}
}

Epoll::~Epoll(void)
{
	if (_fd != -1)
		close(_fd);
}

void	Epoll::addSocketToPoll(ISocket& socket)
{
	struct epoll_event	epoll_event;

	epoll_event.events = EPOLLIN;
	epoll_event.data.ptr = &socket;
	epoll_ctl(_fd, EPOLL_CTL_ADD, socket.getFd(), &epoll_event);
}

void	Epoll::modAddSendEvent(ISocket& socket)
{
	struct epoll_event	epoll_event;

	epoll_event.events = EPOLLIN | EPOLLOUT;
	epoll_event.data.ptr = &socket;
	epoll_ctl(_fd, EPOLL_CTL_MOD, socket.getFd(), &epoll_event);
}

void	Epoll::modRemoveSendEvent(ISocket& socket)
{
	struct epoll_event	epoll_event;

	epoll_event.events = EPOLLIN;
	epoll_event.data.ptr = &socket;
	epoll_ctl(_fd, EPOLL_CTL_MOD, socket.getFd(), &epoll_event);
}

void	Epoll::runEvents(void)
{
	struct epoll_event	events[_MAX_EVENTS];
	int					event_count;

	event_count = epoll_wait(_fd, events, _MAX_EVENTS, 0);
	for (int i = 0; i < event_count; ++i)
		static_cast<ISocket*>(events[i].data.ptr)->handleEvent(events[i].events);
}

const char*	Epoll::CreateException::what(void) const throw()
{
	return ("Epoll::CreateExcpetion");
}
