#ifndef CGIPIPE_HPP
# define CGIPIPE_HPP

# include "ISocket.hpp"
# include "../CGIHandler/CGIHandler.hpp"

class Client;

class CGIPipe : public ISocket
{
	public:
		CGIPipe(Client& client, CGIHandler& cgiHandler);
		~CGIPipe(void);
	private:
		CGIPipe(void);
		CGIPipe(const CGIPipe& other);
		CGIPipe& operator=(const CGIPipe& other);

	public:
		int  getFd(void) const;
		void handleEvent(int events);

	private:
		Client&     _client;
		CGIHandler& _cgiHandler;
		int         _fd;
};

#endif
