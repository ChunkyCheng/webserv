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
		pid_t _activePid;
		int _stdoutPipe[2];
		int _stdinPipe[2];
		std::string _output;
		time_t _startTime;
	
		// Member Functions
		void _closePipe(int pipe[2]);
		char** _buildEnvp(HttpRequest& request);
		std::string _readOutput(int fd);
		HttpResponse _parseCGI(std::string rawOutput);

	public:
		// Constructor & Destructor
		CGIHandler(std::string interpreter, std::string script);
		~CGIHandler();

		// Blocking execution (original)
		// HttpResponse execute(HttpRequest& request);
		
		// Async execution methods
		void executeAsync(HttpRequest& request);
		bool isProcessFinished(int& exitStatus);
		std::string collectOutput();
		HttpResponse parseResponse(const std::string& rawOutput);
		void killProcess();
		pid_t getActivePid() const;
		time_t getStartTime() const;
};

#endif