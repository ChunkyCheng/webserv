#include <iostream>
#include <signal.h>
#include "WebServer.hpp"

static void	sigint(int signum)
{
	(void)signum;
	WebServer::stopServerLoop();
	std::cout << std::endl;
}

int	main(int argc, char *argv[])
{
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
		return (1);
	}
	try
	{
		if (argc == 2)
		{
			WebServer	webserv(argv[1]);
		
			signal(SIGINT, &sigint);
			webserv.runServerLoop();
		}
		else
		{
			WebServer	webserv;
			
			signal(SIGINT, &sigint);
			webserv.runServerLoop();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
