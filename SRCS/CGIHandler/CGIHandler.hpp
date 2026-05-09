/*
 * - takes HttpRequest
 * - return HttpResponse
 * - owns the fork/pipe/execve logic
 * - owns CGI output parsing
 */
#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include <iostream>
# include <unistd.h>
# include <exception>
# include <vector>
# include <sys/wait.h>
# include <cstdlib>

class CGIHandler
{
	private:
		std::string _interpreterPath;
		std::string _scriptPath;
	
		// Member Functions
		void _closePipe(int pipe[2]);
		char** _buildEnvp(HttpRequest& request);
		std::string _readOutput(int fd);
		HttpResponse _parseCGI(std::string rawOutput);

	public:
		// Constructor & Destructor
		CGIHandler(std::string interpreter, std::string script);
		~CGIHandler();

		// Member Functions
		HttpResponse execute(HttpRequest& request);
};

#endif