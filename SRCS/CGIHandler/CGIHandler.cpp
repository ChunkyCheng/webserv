#include "CGIHandler.hpp"
#include "HttpResponse.hpp"
#include <cstring>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

CGIHandler::CGIHandler(std::string interpreter, std::string script) :
_interpreterPath(interpreter), _scriptPath(script), _activePid(-1),
_stdoutReadFd(-1), _stdinWriteFd(-1)
{
	_startTime = 0;
}

CGIHandler::~CGIHandler()
{
	killProcess();
}

// Reads available output from the CGI process without blocking
ssize_t CGIHandler::collectOutputPartial()
{
	if (_stdoutReadFd == -1)
		return 0;

	ssize_t total = 0;
	ssize_t n;
	char buffer[2048];

	while (true) {
		n = read(_stdoutReadFd, buffer, sizeof(buffer));
		if (n > 0) {
			_output.append(buffer, n);
			total += n;
			continue;
		}
		// EOF
		if (n == 0) {
			close(_stdoutReadFd);
			_stdoutReadFd = -1;
			break;
		}
		// No more data for now
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			break;
		}
		// On other errors, close and stop
		close(_stdoutReadFd);
		_stdoutReadFd = -1;
		break;
	}
	return total;
}

const std::string& CGIHandler::getOutput() const
{
	return _output;
}

int CGIHandler::getStdoutFd() const
{
	return (_stdoutReadFd);
}

char** CGIHandler::_buildEnvp(HttpRequest& request) {
	std::vector<std::string> envVars;
	const std::map<std::string, std::string>& headers = request.getHeaders();
	std::string path = request.getPath();
	std::string queryString = "";
	size_t queryPos = path.find("?");
	if (queryPos != std::string::npos) {
		queryString = path.substr(queryPos + 1);
		path = path.substr(0, queryPos);
	}

	envVars.push_back("REQUEST_METHOD=" + request.getMethod());
	envVars.push_back("QUERY_STRING=" + queryString);
	std::map<std::string, std::string>::const_iterator ct = headers.find("content-type");
	envVars.push_back("CONTENT_TYPE=" + (ct == headers.end() ? std::string("") : ct->second));
	{
		std::ostringstream oss;
		oss << request.getContentLength();
		envVars.push_back("CONTENT_LENGTH=" + oss.str());
	}
	envVars.push_back("SCRIPT_NAME=" + path);
	envVars.push_back("SCRIPT_FILENAME=" + _scriptPath);
	envVars.push_back("PATH_INFO=" + path);
	envVars.push_back("SERVER_PROTOCOL=" + request.getVersion());
	envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVars.push_back("SERVER_SOFTWARE=Webserv/1.0");

	// Add HTTP_ headers
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		std::string key = it->first;
		for (size_t i = 0; i < key.length(); ++i) {
			if (key[i] == '-') key[i] = '_';
			else key[i] = std::toupper(key[i]);
		}
		envVars.push_back("HTTP_" + key + "=" + it->second);
	}

	// Convert envVars to char** for execve
	char** envp = new char*[envVars.size() + 1];
	for (size_t i = 0; i < envVars.size(); ++i) {
		envp[i] = new char[envVars[i].size() + 1];
		std::strcpy(envp[i], envVars[i].c_str());
	}
	envp[envVars.size()] = NULL;
	return (envp);
}

HttpResponse CGIHandler::_parseCGI(std::string rawOutput) {
	size_t pos = rawOutput.find("\r\n\r\n");
	std::string lineBreak = "\r\n";
	std::string body = "";
	std::string headers = rawOutput;
	if (pos == std::string::npos) {
		pos = rawOutput.find("\n\n");
		lineBreak = "\n";
	}
	if (pos != std::string::npos) {
		body = rawOutput.substr(pos + (lineBreak.length() * 2));
		headers = rawOutput.substr(0, pos);
	}
	HttpResponse response;
	response.setStatusCode(OK);

	size_t lineStart = 0;
	std::string line;
	while (lineStart < headers.size()) {
		size_t lineEnd = headers.find('\n', lineStart);
		if (lineEnd == std::string::npos)
			line = headers.substr(lineStart);
		else
			line = headers.substr(lineStart, lineEnd - lineStart);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		size_t delimiterPos = line.find(": ");
		if (delimiterPos != std::string::npos) {
			std::string key = line.substr(0, delimiterPos);
			std::string value = line.substr(delimiterPos + 2);
			if (key == "Status") {
				int code = std::atoi(value.substr(0, 3).c_str());
				response.setStatusCode(static_cast<HttpStatus>(code));
			} else {
				response.addHeader(key, value);
			}
		}
		if (lineEnd == std::string::npos)
			break;
		lineStart = lineEnd + 1;
	}

	response.setBody(body);
	return (response);
}


void CGIHandler::executeAsync(HttpRequest& request)
{
	int stdinPipe[2];
	int stdoutPipe[2];

	if (pipe(stdinPipe) == -1) {
		std::cerr << "pipe() failed" << std::endl;
	}
	if (pipe(stdoutPipe) == -1) {
		close(stdinPipe[0]);
		close(stdinPipe[1]);
		std::cerr << "pipe() failed" << std::endl;
	}

	char** envp = _buildEnvp(request);
	pid_t pid = fork();
	if (pid == -1) {
		close(stdinPipe[0]);
		close(stdinPipe[1]);
		close(stdoutPipe[0]);
		close(stdoutPipe[1]);
		for (size_t i = 0; envp[i]; ++i) delete[] envp[i];
		delete[] envp;
		std::cerr << "fork() failed" << std::endl;
	}
	if (pid == 0) {
		close(stdinPipe[1]);
		close(stdoutPipe[0]);
		dup2(stdinPipe[0], STDIN_FILENO);
		dup2(stdoutPipe[1], STDOUT_FILENO);
		close(stdinPipe[0]);
		close(stdoutPipe[1]);
		// Change to script directory for relative file access
		size_t last_slash = this->_scriptPath.find_last_of("/\\");
		std::string script_name = this->_scriptPath;
		if (last_slash != std::string::npos) {
			std::string script_dir = this->_scriptPath.substr(0, last_slash);
			if (chdir(script_dir.c_str()) != 0) {
				std::cerr << "chdir() failed" << std::endl;
				exit(EXIT_FAILURE);
			}
			script_name = this->_scriptPath.substr(last_slash + 1);
		}
		char* arr[3];
		arr[0] = const_cast<char*>(this->_interpreterPath.c_str());
		arr[1] = const_cast<char*>(script_name.c_str());
		arr[2] = NULL;
		execve(arr[0], arr, envp);
		std::cerr << "execve() failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; envp[i]; ++i) delete[] envp[i];
	delete[] envp;

	// Parent: close the child-side ends
	close(stdinPipe[0]);
	close(stdoutPipe[1]);

	_stdinWriteFd = stdinPipe[1];
	_stdoutReadFd = stdoutPipe[0];
	_activePid = pid;
	_startTime = time(NULL);

	// Mark parent-side fds close-on-exec and make stdout non-blocking
	if (_stdinWriteFd != -1) {
		int flags = fcntl(_stdinWriteFd, F_GETFD);
		if (flags != -1) fcntl(_stdinWriteFd, F_SETFD, flags | FD_CLOEXEC);
	}
	if (_stdoutReadFd != -1) {
		int flags = fcntl(_stdoutReadFd, F_GETFD);
		if (flags != -1) fcntl(_stdoutReadFd, F_SETFD, flags | FD_CLOEXEC);
		int oflags = fcntl(_stdoutReadFd, F_GETFL, 0);
		if (oflags != -1) fcntl(_stdoutReadFd, F_SETFL, oflags | O_NONBLOCK);
	}

	// Write POST body if present
	if (request.getMethod() == "POST") {
		const std::string& body = request.getBody();
		if (!body.empty()) {
			ssize_t written = write(_stdinWriteFd, body.c_str(), body.size());
			(void)written;
		}
	}
	close(_stdinWriteFd);
	_stdinWriteFd = -1;
}

bool CGIHandler::isProcessFinished(int& exitStatus)
{
	if (_activePid <= 0)
		return (true);

	int status = 0;
	pid_t ret = waitpid(_activePid, &status, WNOHANG);

	if (ret == -1)
	{
		_activePid = -1;
		return (true);
	}
	if (ret == 0)
		return (false);

	_activePid = -1;
	exitStatus = status;
	return (true);
}

void CGIHandler::killProcess()
{
	if (_activePid > 0)
	{
		kill(_activePid, SIGKILL);
		waitpid(_activePid, NULL, 0);
		_activePid = -1;
	}
	if (_stdoutReadFd != -1)
	{
		close(_stdoutReadFd);
		_stdoutReadFd = -1;
	}
	if (_stdinWriteFd != -1)
	{
		close(_stdinWriteFd);
		_stdinWriteFd = -1;
	}
}

pid_t CGIHandler::getActivePid() const
{
	return (_activePid);
}

time_t CGIHandler::getStartTime() const
{
	return (_startTime);
}

HttpResponse CGIHandler::parseResponse(const std::string& rawOutput)
{
	return (_parseCGI(rawOutput));
}
