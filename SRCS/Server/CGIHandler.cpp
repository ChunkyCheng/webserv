#include "CGIHandler.hpp"
#include "CGIStdinPipe.hpp"
#include "CGIStdoutPipe.hpp"
#include "Epoll.hpp"
#include "ISocket.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

static char** buildArgv(const std::string& interpreter, const std::string& script_path) {
	char** argv = new char*[3];
	argv[0] = new char[interpreter.size() + 1];
	std::strcpy(argv[0], interpreter.c_str());
	argv[1] = new char[script_path.size() + 1];
	std::strcpy(argv[1], script_path.c_str());
	argv[2] = NULL;
	return argv;
}

static void freeArgv(char** argv) {
	for (int i = 0; argv[i] != NULL; ++i) {
		delete[] argv[i];
	}
	delete[] argv;
}

static char** buildEnvp(const std::vector<std::string>& env) {
	char** envp = new char*[env.size() + 1];
	for (size_t i = 0; i < env.size(); ++i) {
		envp[i] = new char[env[i].size() + 1];
		std::strcpy(envp[i], env[i].c_str());
	}
	envp[env.size()] = NULL;
	return envp;
}

static void freeEnvp(char** envp) {
	for (int i = 0; envp[i] != NULL; ++i) {
		delete[] envp[i];
	}
	delete[] envp;
}

CGIHandler::CGIHandler(
	const std::string& interpreter,
	const std::string& script_path,
	const std::string& working_dir,
	const std::string& request_body,
	const std::vector<std::string>& env,
	Epoll& epoll,
	ISocket& client_socket
)
	: _pid(-1), _stdin_fd(-1), _stdout_fd(-1),
	  _request_body(request_body), _bytes_written(0),
	  _start_time(std::time(NULL)),
	  _error(false), _complete(false),
	  _epoll(epoll), _client_socket(client_socket),
	  _stdin_pipe(NULL), _stdout_pipe(NULL)
{
	int stdin_fds[2];
	int stdout_fds[2];

	if (pipe(stdin_fds) == -1 || pipe(stdout_fds) == -1) {
		_error = true;
		_complete = true;
		std::cerr << "Failed to create pipes for CGI: " << strerror(errno) << std::endl;
		return;
	}

	char** argv = buildArgv(interpreter, script_path);
	char** envp = buildEnvp(env);

	_pid = fork();
	if (_pid == -1) {
		freeArgv(argv);
		freeEnvp(envp);
		close(stdin_fds[0]); close(stdin_fds[1]);
		close(stdout_fds[0]); close(stdout_fds[1]);
		std::cerr << "Failed to fork CGI process: " << strerror(errno) << std::endl;
		_error = true;
		_complete = true;
		return;
	}

	if (_pid == 0) {
		dup2(stdin_fds[0], STDIN_FILENO);
		dup2(stdout_fds[1], STDOUT_FILENO);
		close(stdin_fds[0]); close(stdin_fds[1]);
		close(stdout_fds[0]); close(stdout_fds[1]);
		chdir(working_dir.c_str());
		execve(argv[0], argv, envp);
		std::cerr << "Failed to execute CGI script: " << strerror(errno) << std::endl;
		exit(1);
	}

	/* Parent */
	freeArgv(argv);
	freeEnvp(envp);
	close(stdin_fds[0]);
	close(stdout_fds[1]);

	_stdin_fd = stdin_fds[1];
	_stdout_fd = stdout_fds[0];

	fcntl(_stdin_fd, F_SETFL, O_NONBLOCK);
	fcntl(_stdout_fd, F_SETFL, O_NONBLOCK);

	if (!_request_body.empty()) {
		_stdin_pipe = new CGIStdinPipe(_stdin_fd, *this);
		_epoll.addSocketWithEvent(*_stdin_pipe, EPOLLOUT);
	}
	else {
		close(_stdin_fd);
		_stdin_fd = -1;
	}

	_stdout_pipe = new CGIStdoutPipe(_stdout_fd, *this);
	_epoll.addSocketWithEvent(*_stdout_pipe, EPOLLIN);
}