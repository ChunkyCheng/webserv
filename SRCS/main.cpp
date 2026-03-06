#include <iostream>
#include "Server.hpp"

int	main(int argc, char *argv[])
{
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
		return (1);
	}
	Server	server1;
}
