#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP


# include <string>
# include <vector>
# include <ctime>


class Epoll;
class ISocket;
class CGIStdinPipe;
class CGIStdoutPipe;

class	CGIHandler
{
	public:
		CGIHandler(
			const std::string& interpreter,
			const std::string& script_path,
			const std::string& working_dir,
			const std::string& request_body,
			const std::vector<std::string>& env,
			Epoll& epoll,
			ISocket& client_socket
		);
		~CGIHandler(void);

		const std::string&	getOutput(void) const;
		const time_t&		getStartTime(void) const;
		const pid_t&		getPid(void) const;
		void				onStdinReady(void);
		void				onStdoutReady(void);
		void				onStdoutHup(void);
		bool				hasError(void) const;
		bool				isComplete(void) const;
		void				killProcess(void);

	private:
		CGIHandler(void);
		CGIHandler(const CGIHandler& other);
		CGIHandler&	operator=(const CGIHandler& other);

		pid_t			_pid;
		int				_stdin_fd;
		int				_stdout_fd;
		std::string		_request_body;
		size_t			_bytes_written;
		std::string		_cgi_output;
		time_t			_start_time;
		bool			_error;
		bool			_complete;
		Epoll&			_epoll;
		ISocket&		_client_socket;
		CGIStdinPipe*	_stdin_pipe;
		CGIStdoutPipe*	_stdout_pipe;
};

#endif