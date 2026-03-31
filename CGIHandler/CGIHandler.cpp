#include "CGIHandler.hpp"

CGIHandler::CGIHandler(HttpRequest request, std::string interpreter, std::string script) :
_interpreterPath(interpreter), _scriptPath(script), _request(request) {}

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

/*
 * Example CGI output:
 *
 * Status: 404 Not Found\r\n
 * Content-Type: text/html\r\n
 * \r\n
 * <h1>Page not found</h1>
 */
HttpResponse CGIHandler::_parseCGI(std::string rawOutput) {
	size_t pos = rawOutput.find("\r\n\r\n");
	std::string body = rawOutput.substr(pos + 4);
	std::string headers = rawOutput.substr(0, pos);
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
		std::string key = line.substr(0, delimiterPos);
		std::string value = line.substr(delimiterPos + 2);
		if (key == "Status") {
			response.statusCode = std::atoi(value.c_str());
		} else {
			response.headers[key] = value;
		}
		if (lineEnd == std::string::npos)
			break ;
		lineStart = lineEnd + 2;
		lineEnd = headers.find("\r\n", lineStart);
	}

	response.body = body;
	return (response);
}


HttpResponse CGIHandler::execute() {
	int stdinPipe[2]; // parent writes, child reads
	int stdoutPipe[2]; // parent reads, child writes

	if (pipe(stdinPipe) == -1) {
		throw std::runtime_error("pipe() failed");
	}
	if (pipe(stdoutPipe) == -1) {
		_closePipe(stdinPipe);
		throw std::runtime_error("pipe() failed");
	}
	pid_t pid = fork();
	if (pid == -1) {
		_closePipe(stdinPipe);
		_closePipe(stdoutPipe);
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
		execve(arr[0], arr, NULL);
		std::cerr << "execve() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	close(stdinPipe[0]);
	close(stdoutPipe[1]);
	if (_request.getMethod() == "POST") {
		const std::string& body = _request.getBody();
		ssize_t written = write(stdinPipe[1], body.c_str(), body.size());
		if (written == -1 || static_cast<size_t>(written) != body.size()) {
			close(stdinPipe[1]);
			close(stdoutPipe[0]);
			waitpid(pid, NULL, 0);
			throw std::runtime_error("failed to write request body to CGI stdin");
		}
	}
	close(stdinPipe[1]);
	std::string rawOutput = _readOutput(stdoutPipe[0]);
	waitpid(pid, NULL, 0);
	close(stdoutPipe[0]);
	HttpResponse response = _parseCGI(rawOutput);
	return (response);
}