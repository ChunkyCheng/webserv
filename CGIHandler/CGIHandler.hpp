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

class CGIHandler
{
	public:
		// Constructor & Destructor
		CGIHandler(HttpRequest& request, std::string& interpreter, std::string& script);
		~CGIHandler();

		// Member Functions

	private:
		std::string _interpreterPath;
		std::string _scriptPath;
		HttpRequest _request;
};

#endif