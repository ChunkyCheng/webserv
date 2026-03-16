#ifndef SOCKET_H
# define SOCKET_H

# include <fcntl.h>
# include <unistd.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <cerrno>
# include <cstring>
# include <string>
# include <iostream>
# include <vector>
# include <map>

class	WebServer;
class	Epoll;
class	Server;
class	Client;
class	ServerSocket;
class	ClientSocket;

#endif
