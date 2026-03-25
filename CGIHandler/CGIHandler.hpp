/*
 * - takes HttpRequest
 * - return HttpResponse
 * - owns the fork/pipe/execve logic
 * - owns CGI output parsing
 */
#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "request.hpp"
# include <iostream>
# include <unistd.h>
# include <exception>
# include <vector>
# include <sys/wait.h>
# include <cstdlib>

struct HttpResponse {
	int statusCode;
	std::map<std::string, std::string> headers;
	std::string body;
};

class CGIHandler
{
	private:
		std::string _interpreterPath;
		std::string _scriptPath;
		HttpRequest _request;
		
		// Member Functions
		void closePipe(int pipe[2]);
		std::string readOutput(int fd);

	public:
		// Constructor & Destructor
		CGIHandler(HttpRequest request, std::string interpreter, std::string script);
		~CGIHandler();

		// Member Functions
		HttpResponse execute();
};

#endif