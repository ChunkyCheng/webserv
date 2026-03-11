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

void	Epoll::addServerToPoll(Server& server)
{
	const std::vector<ServerSocket*>&	listeningSockets = server.getListeningSockets();
	struct epoll_event			epoll_event;

	epoll_event.events = EPOLLIN;
	for (unsigned int i = 0; i < listeningSockets.size(); ++i)
	{
		epoll_event.data.ptr = listeningSockets[i];
		epoll_ctl(_fd, EPOLL_CTL_ADD, listeningSockets[i]->getFd(), &epoll_event);
	}
}

void	Epoll::addClientToPoll(ClientSocket& client_socket)
{
	struct epoll_event	epoll_event;

	epoll_event.events = EPOLLIN | EPOLLOUT;
	epoll_event.data.ptr = &client_socket;
	epoll_ctl(_fd, EPOLL_CTL_ADD, client_socket.getFd(), &epoll_event);
}

void	Epoll::runEvents(void)
{
	struct epoll_event	events[_MAX_EVENTS];
	int					event_count;

	event_count = epoll_wait(_fd, events, _MAX_EVENTS, 0);
	for (int i = 0; i < event_count; ++i)
		static_cast<ISocket*>(events[i].data.ptr)->handleEvent();
}

const char*	Epoll::CreateException::what(void) const throw()
{
	return ("Epoll::CreateExcpetion");
}
