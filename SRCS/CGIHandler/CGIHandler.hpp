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
		std::string	_interpreterPath;
		std::string	_scriptPath;
		pid_t		_activePid;
		int			_stdoutReadFd;
		int			_stdinWriteFd;
		std::string _output;
		time_t 		_startTime;

		// Member Functions
		char** _buildEnvp(HttpRequest& request);
		HttpResponse _parseCGI(std::string rawOutput);

	public:
		// Constructor & Destructor
		CGIHandler(std::string interpreter, std::string script);
		~CGIHandler();

		// Async execution methods
		void				executeAsync(HttpRequest& request);
		bool				isProcessFinished(int& exitStatus);
		ssize_t				collectOutputPartial();
		const std::string&	getOutput() const;
		int					getStdoutFd() const;
		HttpResponse		parseResponse(const std::string& rawOutput);
		void 				killProcess();
		pid_t 				getActivePid() const;
		time_t				getStartTime() const;
};

#endif
