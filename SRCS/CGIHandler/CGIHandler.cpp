#include "CGIHandler.hpp"
#include "HttpResponse.hpp"
#include <cstring>
#include <sstream>
#include <algorithm>

CGIHandler::CGIHandler(std::string interpreter, std::string script) :
_interpreterPath(interpreter), _scriptPath(script) {}

CGIHandler::~CGIHandler() {}

void CGIHandler::_closePipe(int pipe[2]) {
	close(pipe[0]);
	close(pipe[1]);
}

std::string CGIHandler::_readOutput(int fd) {
	ssize_t bytes;
	char buffer[2048];
	std::string output;

	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
		output.append(buffer, bytes);
	}
	return (output);
}

static std::string sizeToString(size_t number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

char** CGIHandler::_buildEnvp(HttpRequest& request) {
	std::vector<std::string> envVars;
	std::string path = request.getPath();
	std::string queryString = "";
	size_t queryPos = path.find("?");
	if (queryPos != std::string::npos) {
		queryString = path.substr(queryPos + 1);
		path = path.substr(0, queryPos);
	}

	envVars.push_back("REQUEST_METHOD=" + request.getMethod());
	envVars.push_back("QUERY_STRING=" + queryString);
	envVars.push_back("CONTENT_TYPE=" + request.getHeaders()["Content-Type"]);
	envVars.push_back("CONTENT_LENGTH=" + sizeToString(request.getContentLength()));
	envVars.push_back("SCRIPT_NAME=" + path);
	envVars.push_back("SCRIPT_FILENAME=" + _scriptPath);
	envVars.push_back("PATH_INFO=" + path);
	envVars.push_back("SERVER_PROTOCOL=" + request.getVersion());
	envVars.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envVars.push_back("SERVER_SOFTWARE=Webserv/1.0");

	// Add HTTP_ headers
	std::map<std::string, std::string> headers = request.getHeaders();
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
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
	std::string body = "";
	std::string headers = rawOutput;
	if (pos != std::string::npos) {
		body = rawOutput.substr(pos + 4);
		headers = rawOutput.substr(0, pos);
	}
	HttpResponse response;

	size_t lineStart = 0;
	size_t lineEnd = headers.find("\r\n");
	std::string line;
	while (lineStart < headers.size()) {
		if (lineEnd == std::string::npos) {
			line = headers.substr(lineStart);
		} else {
			line = headers.substr(lineStart, lineEnd - lineStart);
		}
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
			break ;
		lineStart = lineEnd + 2;
		lineEnd = headers.find("\r\n", lineStart);
	}

	response.setBody(body);
	return (response);
}


HttpResponse CGIHandler::execute(HttpRequest& request) {
	int stdinPipe[2]; // parent writes, child reads
	int stdoutPipe[2]; // parent reads, child writes

	if (pipe(stdinPipe) == -1) {
		throw std::runtime_error("pipe() failed");
	}
	if (pipe(stdoutPipe) == -1) {
		_closePipe(stdinPipe);
		throw std::runtime_error("pipe() failed");
	}
	
	char** envp = _buildEnvp(request);
	pid_t pid = fork();
	if (pid == -1) {
		_closePipe(stdinPipe);
		_closePipe(stdoutPipe);
		for (size_t i = 0; envp[i]; ++i) delete[] envp[i];
		delete[] envp;
		throw std::runtime_error("fork() failed");
	}
	if (pid == 0) {
		close(stdinPipe[1]);
		close(stdoutPipe[0]);
		dup2(stdinPipe[0], STDIN_FILENO);
		dup2(stdoutPipe[1], STDOUT_FILENO);
		close(stdinPipe[0]);
		close(stdoutPipe[1]);
		char* arr[3];
		arr[0] = const_cast<char*>(this->_interpreterPath.c_str());
		arr[1] = const_cast<char*>(this->_scriptPath.c_str());
		arr[2] = NULL;
		execve(arr[0], arr, envp);
		std::cerr << "execve() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	for (size_t i = 0; envp[i]; ++i) delete[] envp[i];
	delete[] envp;

	close(stdinPipe[0]);
	close(stdoutPipe[1]);
	if (request.getMethod() == "POST") {
		const std::string& body = request.getBody();
		if (!body.empty()) {
			ssize_t written = write(stdinPipe[1], body.c_str(), body.size());
			if (written == -1 || static_cast<size_t>(written) != body.size()) {
				close(stdinPipe[1]);
				close(stdoutPipe[0]);
				waitpid(pid, NULL, 0);
				throw std::runtime_error("failed to write request body to CGI stdin");
			}
		}
	}
	close(stdinPipe[1]);
	std::string rawOutput = _readOutput(stdoutPipe[0]);
	waitpid(pid, NULL, 0);
	close(stdoutPipe[0]);
	HttpResponse response = _parseCGI(rawOutput);
	return (response);
}