#include "RequestHandler.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include "CGIHandler.hpp"

RequestHandler::RequestHandler(Server& server,
std::string& req_buff, std::string& res_buff)
	: _server(server),
	_request_buff(req_buff),
	_response_buff(res_buff),
	_httpRequest(),
	_req_state(REQ_READING_HEADERS),
	_res_state(RES_HEADERS),
	_handler_error_code(NONE),
	_location(NULL),
	_should_close_connection(false),
	_cgi_process_pid(-1),
	_cgi_start_time(0),
	_cgi_handler(NULL)
{
}

RequestHandler::~RequestHandler(void)
{
	if (_cgi_handler)
	{
		delete _cgi_handler;
		_cgi_handler = NULL;
	}
}

bool	RequestHandler::checkRequestComplete(void) const
{
	return (_req_state == REQ_COMPLETE || _req_state == REQ_ERROR);
}

bool	RequestHandler::checkResponseComplete(void) const
{
	return (_res_state == RES_FINISHED);
}

bool	RequestHandler::isCgiWaiting(void) const
{
	return (_res_state == RES_CGI_WAITING);
}

void	RequestHandler::processReqData(void)
{
	bool	keep_processing = true;

	while (keep_processing)
	{
		switch (_req_state)
		{
			case REQ_READING_HEADERS:
			{
				if (!_httpRequest.parseHeaders(_request_buff))
				{
					if (_httpRequest.hasError())
						_req_state = REQ_ERROR;
					else
						keep_processing = false;
					break;
				}
				HttpStatus validation_status = validateRequestLocation();
				if (validation_status != OK)
				{
					_handler_error_code = validation_status;
					_req_state = REQ_ERROR;
					break;
				}
				_req_state = _httpRequest.hasBody() ? REQ_READING_BODY : REQ_COMPLETE;
				break;
			}

			case REQ_READING_BODY:
				if (_httpRequest.parseBody(_request_buff))
					_req_state = REQ_COMPLETE;
				else
					if (_httpRequest.hasError())
						_req_state = REQ_ERROR;
					else
						keep_processing = false;
				break;

			case REQ_COMPLETE:
				keep_processing = false;
				break;
			
			case REQ_ERROR:
				if (_httpRequest.hasError() || _handler_error_code == PAYLOAD_TOO_LARGE)
				{
					_should_close_connection = true;
					_request_buff.clear();
				}
				keep_processing = false;
				break;
		}
	}
}

void	RequestHandler::buildResponseData(void)
{
	HttpStatus status = resolveInitialStatus();
	evaluateConnectionState(status);
	if (status == NONE)
	{
		status = executeMethod();
	}
	if (status != NONE)
	{
		buildErrorOrRedirectResponse(status);
	}
	// Only assemble final buffer if we're not waiting for CGI
	if (_res_state != RES_CGI_WAITING)
	{
		assembleFinalBuffer();
	}
}

// void    RequestHandler::buildResponseData(void)
// {
// 	HttpStatus final_error = NONE;
// 	std::string body_content;
// 	std::string physical_path;
// 	std::string method;
	
// 	if (_httpRequest.hasError())
// 	{
// 		final_error = _httpRequest.getError();
// 	}
// 	else if (_handler_error_code != NONE)
// 	{
// 		final_error = _handler_error_code;
// 	}
// 	if (!_httpRequest.wantsKeepAlive() ||
// 	_httpRequest.hasError() ||
// 	final_error == PAYLOAD_TOO_LARGE)
// 	{
// 		_should_close_connection = true;
// 	}
// 	if (final_error == NONE)
// 	{
// 		physical_path = getNormalPagePath();
// 		method = _httpRequest.getMethod();
// 		if (method == "GET")
// 		{
// 			handleGetMethod(physical_path);
// 	}
// 	// else if (method == "POST")
// 	// {
// 		// 	handlePostMethod(physical_path);
// 		// }
// 		// else if (method == "DELETE")
// 		// {
// 			// 	handleDeleteMethod(physical_path);
// 			// }
// 			else
// 			{
// 				final_error = METHOD_NOT_ALLOWED;
// 			}
// 		}
// 		if (_handler_error_code != NONE)
// 		{
// 			final_error = _handler_error_code;
// 		}
// 		if (final_error != NONE)
// 		{
// 			if (final_error == MOVED_PERMANENTLY ||
// 				final_error == FOUND ||
// 				final_error == SEE_OTHER ||
// 				final_error == TEMPORARY_REDIRECT ||
// 				final_error == PERMANENT_REDIRECT)
// 				{
// 					std::string target;
// 					if (!_redirect_target.empty())
// 					{
// 						target = _redirect_target;
// 					}
// 					else
// 					{
// 						target = _location->getReturnTarget();
// 					}
// 		_httpResponse.buildRedirectHeaders(target, final_error);
// 		}
// 		else
// 		{
// 			body_content.clear();
// 			physical_path = getErrorPagePath(final_error);
// 			readFileContent(physical_path, body_content);
// 			_httpResponse.buildErrorPage(final_error, body_content);
// 		}
// 	}
// 	if (_should_close_connection)
// 		_httpResponse.addHeader("Connection", "close");
// 	else
// 		_httpResponse.addHeader("Connection", "keep-alive");
// 	_response_buff = _httpResponse.getFormattedHeaders();
// 	const std::string& memory_body = _httpResponse.getBody();
// 	if (!memory_body.empty())
// 	{
// 		_response_buff += memory_body;
// 	}
// }

void	RequestHandler::continueBuildResponse(void)
{
	// Handle CGI_WAITING state - check if process is finished
	if (_res_state == RES_CGI_WAITING)
	{
		if (!_cgi_handler)
		{
			_handler_error_code = INTERNAL_SERVER_ERROR;
			_res_state = RES_FINISHED;
			buildErrorOrRedirectResponse(_handler_error_code);
			assembleFinalBuffer();
			return;
		}
		
		// Check for timeout
		time_t elapsed = time(NULL) - _cgi_start_time;
		if (elapsed > CGI_TIMEOUT)
		{
			_cgi_handler->killProcess();
			_handler_error_code = INTERNAL_SERVER_ERROR;
			_res_state = RES_FINISHED;
			buildErrorOrRedirectResponse(_handler_error_code);
			assembleFinalBuffer();
			return;
		}
		
		// Check if process finished
		int exitStatus = 0;
		if (_cgi_handler->isProcessFinished(exitStatus))
		{
			// Collect output and parse response
			std::string rawOutput = _cgi_handler->collectOutput();
			_httpResponse = _cgi_handler->parseResponse(rawOutput);
			
			// Assemble the buffer now that we have the response
			if (_should_close_connection)
				_httpResponse.addHeader("Connection", "close");
			else
				_httpResponse.addHeader("Connection", "keep-alive");
			_response_buff = _httpResponse.getFormattedHeaders();
			const std::string& memory_body = _httpResponse.getBody();
			if (!memory_body.empty())
			{
				_response_buff += memory_body;
			}
			
			// Transition to finished state (response is complete for CGI)
			_res_state = RES_FINISHED;
			
			// Clean up CGI handler
			delete _cgi_handler;
			_cgi_handler = NULL;
		}
		// If still running, don't transition state - will be checked again next call
		return;
	}
	
	// Handle normal header/body streaming
	if (_res_state == RES_HEADERS)
	{
		_res_state = RES_BODY;
	}
	if (_res_state == RES_BODY)
	{
		char buffer[8192];
		if (_file_stream.is_open())
		{
			_file_stream.read(buffer, sizeof(buffer));
			std::streamsize bytes_read = _file_stream.gcount();
			if (bytes_read > 0)
			{
				_response_buff.append(buffer, bytes_read);
			}
			if (_file_stream.bad())
			{
				_file_stream.close();
				_should_close_connection = true;
				_res_state = RES_FINISHED;
			}
			else if (_file_stream.eof())
			{
				_file_stream.close();
				_res_state = RES_FINISHED;
				
			}
		}
		else
		{
			_res_state = RES_FINISHED;
			_should_close_connection = true;
		}
	}
}

// ==============================================================================
// BUILDRESPONSEDATA PRIVATE FUNCTIONS
// ==============================================================================

void RequestHandler::assembleFinalBuffer()
{
	if (_should_close_connection)
		_httpResponse.addHeader("Connection", "close");
	else
		_httpResponse.addHeader("Connection", "keep-alive");
	_response_buff = _httpResponse.getFormattedHeaders();
	const std::string& memory_body = _httpResponse.getBody();
	if (!memory_body.empty())
	{
		_response_buff += memory_body;
	}
}

void	RequestHandler::buildErrorOrRedirectResponse(HttpStatus status)
{
	bool is_redirect = (status == MOVED_PERMANENTLY || status == FOUND ||
						status == SEE_OTHER || status == TEMPORARY_REDIRECT ||
						status == PERMANENT_REDIRECT);
		
	if (is_redirect)
	{
		std::string target = !_redirect_target.empty() ? _redirect_target : _location->getReturnTarget();
		_httpResponse.buildRedirectHeaders(target, status);
	}
	else
	{
		std::string body_content;
		std::string physical_path = getErrorPagePath(status);
		readFileContent(physical_path, body_content);
		_httpResponse.buildErrorPage(status, body_content);
	}
}

HttpStatus	RequestHandler::executeMethod()
{
		std::string physical_path = getNormalPagePath();
		std::string method = _httpRequest.getMethod();
		if (method == "GET")
		{
			handleGetMethod(physical_path);
		}
		else if (method == "POST")
		{
			handlePostMethod(physical_path);
		}
		else if (method == "DELETE")
		{
			handleDeleteMethod(physical_path);
		}
		else
		{
			return (METHOD_NOT_ALLOWED);
		}
		return (_handler_error_code != NONE ? _handler_error_code : NONE);
}


HttpStatus RequestHandler::resolveInitialStatus()
{
	if (_httpRequest.hasError())
	{
		return (_httpRequest.getError());
	}
	if (_handler_error_code != NONE)
	{
		return (_handler_error_code);
	}
	return (NONE);
}

void	RequestHandler::evaluateConnectionState(HttpStatus status)
{
	if (!_httpRequest.wantsKeepAlive() || _httpRequest.hasError() || status == PAYLOAD_TOO_LARGE)
	{
		_should_close_connection = true;
	}
}

HttpStatus RequestHandler::validateRequestLocation()
{
	std::string uri = _httpRequest.getPath();
	std::cout << "PATH: " << uri << "\n";
	const std::vector<Location>& bookshelf = _server.getLocations();
	_location = matchLocation(uri, bookshelf);
	if (!_location)
		return (NOT_FOUND);
	else if (_location->getReturnCode() > 0)
		return (static_cast<HttpStatus>(_location->getReturnCode()));
	else if (!_location->isMethodAllowed(_httpRequest.getMethod()))
		return (METHOD_NOT_ALLOWED);
	else if (_httpRequest.hasBody() && _httpRequest.getContentLength() > _location->getClientMaxBodySize())
		return (PAYLOAD_TOO_LARGE);
	return (OK);
}

bool	RequestHandler::readFileContent(const std::string& file_path, std::string& body_content)
{
	std::ifstream file(file_path.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		return (false);
	}
	file.seekg(0, std::ios::end);
	std::streamsize file_size = file.tellg();
	file.seekg(0, std::ios::beg);
	if (file_size > 0)
	{
		body_content.resize(file_size);
		file.read(&body_content[0], file_size);
	}
	else
	{
		body_content.clear();
	}
	return (true);
}

const Location* RequestHandler::getDefaultLocation(void) const
{
	const std::vector<Location>& bookshelf = _server.getLocations();
	for (std::vector<Location>::const_iterator it = bookshelf.begin(); it != bookshelf.end(); ++it)
	{
		if (it->getPrefix() == "/")
		{
			return (&(*it));
		}
	}
	return (NULL);
}

std::string RequestHandler::getErrorPagePath(HttpStatus error_code)
{
	const Location* target_location = _location;
	if (!target_location)
	{
		target_location = getDefaultLocation();
	}
	if (!target_location)
	{
		return ("");
	}
	const std::map<int, std::string>& error_pages = target_location->getErrorPages();
	std::map<int, std::string>::const_iterator it = error_pages.find(error_code);
	if (it == error_pages.end())
	{
		return ("");
	}
	std::string custom_path = it->second;
	// This will be commented out because config returns absolute path for error pages for now
	// std::string root = target_location->getRoot();
	// if (!root.empty() && root[root.length() - 1] != '/' && !custom_path.empty() && custom_path[0] != '/')
	// {
		// 	root += "/";
		// }
		// std::string full_path = root + custom_path;
		// std::cout << "Error page full path: " << full_path << "\n";
		struct stat file_stat;
		if (stat(custom_path.c_str(), &file_stat) == 0)
		{
			if (S_ISREG(file_stat.st_mode))
			{
				return (custom_path);
			}
		}
		return ("");
	}
	
std::string RequestHandler::getNormalPagePath(void)
{
	std::string root = _location->getRoot();
	std::string prefix = _location->getPrefix();
	std::string uri = _httpRequest.getPath();
	
	// Remove query string from URI
	std::string::size_type query_pos = uri.find('?');
	if (query_pos != std::string::npos)
	{
		uri = uri.substr(0, query_pos);
	}
	
	// Extract suffix after the location prefix
	std::string suffix = uri;
	if (uri.find(prefix) == 0)  // If URI starts with prefix
	{
		suffix = uri.substr(prefix.length());
		if (suffix.empty())
			suffix = "/";
	}
	
	// Normalize path separators between root and URI suffix
	if (!root.empty() && root[root.length() - 1] == '/' && !suffix.empty() && suffix[0] == '/')
	{
		root.erase(root.length() - 1);
	}
	else if (!root.empty() && root[root.length() - 1] != '/' && !suffix.empty() && suffix[0] != '/')
	{
		root += "/";
	}
	
	std::string full_path = root + suffix;
	return (full_path);
}

bool	RequestHandler::isCgiRequest(const std::string& physical_path, Config::s_cgi_info& info) const
{
	const std::map<std::string, Config::s_cgi_info>& cgi_info = _location->getCgiInfo();
	std::string request_path = physical_path;
	std::string::size_type query_pos = request_path.find('?');
	if (query_pos != std::string::npos)
	{
		request_path = request_path.substr(0, query_pos);
	}
	std::string::size_type pos = request_path.find_last_of('.');

	if (pos == std::string::npos)
		return (false);
	std::string extension = request_path.substr(pos);
	std::map<std::string, Config::s_cgi_info>::const_iterator it = cgi_info.find(extension);
	if (it == cgi_info.end())
		return (false);
	info = it->second;
	return (true);
}

void	RequestHandler::handleCgiMethod(const std::string& physical_path)
{
	Config::s_cgi_info info;

	if (!isCgiRequest(physical_path, info))
	{
		_handler_error_code = NOT_FOUND;
		return ;
	}
	if (info.exec_path.empty())
	{
		_handler_error_code = NOT_IMPLEMENTED;
		return ;
	}
	if (access(physical_path.c_str(), R_OK) != 0 || access(info.exec_path.c_str(), X_OK) != 0)
	{
		_handler_error_code = FORBIDDEN;
		return ;
	}
	try
	{
		if (_cgi_handler)
		{
			delete _cgi_handler;
			_cgi_handler = NULL;
		}
		_cgi_handler = new CGIHandler(info.exec_path, physical_path);
		_cgi_handler->executeAsync(_httpRequest);
		_cgi_process_pid = _cgi_handler->getActivePid();
		_cgi_start_time = _cgi_handler->getStartTime();
		_res_state = RES_CGI_WAITING;
	}
	catch (std::exception&)
	{
		_handler_error_code = INTERNAL_SERVER_ERROR;
		if (_cgi_handler)
		{
			delete _cgi_handler;
			_cgi_handler = NULL;
		}
	}
}


void RequestHandler::handlePostMethod(const std::string& physical_path)
{
	Config::s_cgi_info info;

	if (isCgiRequest(physical_path, info))
	{
		handleCgiMethod(physical_path);
		return ;
	}
	_handler_error_code = NOT_IMPLEMENTED;
}

void RequestHandler::handleDeleteMethod(const std::string& physical_path)
{
	struct stat file_stat;

	if (stat(physical_path.c_str(), &file_stat) != 0)
	{
		_handler_error_code = NOT_FOUND;
		return ;
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		_handler_error_code = FORBIDDEN;
		return ;
	}
	if (access(physical_path.c_str(), W_OK) != 0)
	{
		_handler_error_code = FORBIDDEN;
		return ;
	}
	if (unlink(physical_path.c_str()) != 0)
	{
		_handler_error_code = INTERNAL_SERVER_ERROR;
		return ;
	}
	_httpResponse.setStatusCode(NO_CONTENT);
	_httpResponse.addHeader("Content-Length", "0");
}

void RequestHandler::handleGetMethod(const std::string& physical_path)
{
	Config::s_cgi_info info;
	struct stat file_stat;
	if (stat(physical_path.c_str(), &file_stat) != 0)
	{
		_handler_error_code = NOT_FOUND;
		return;
	}
	if (access(physical_path.c_str(), R_OK) != 0)
	{
		_handler_error_code = FORBIDDEN;
		return;
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		std::string uri = _httpRequest.getPath();
		if (!uri.empty() && uri[uri.length() - 1] != '/')
		{
			_handler_error_code = MOVED_PERMANENTLY;
			_redirect_target = uri + "/";
			return;
		}
		bool index_found = false;
		std::string final_index_path = "";
		const std::vector<std::string>& index = _location->getIndex();
		for (size_t i = 0; i < index.size(); ++i)
		{
			std::string test_path = physical_path + index[i];
			if (access(test_path.c_str(), R_OK) == 0)
			{
				index_found = true;
				final_index_path = test_path;
				break;
			}
		}
		if (index_found)
		{
			if (isCgiRequest(final_index_path, info))
			{
				handleCgiMethod(final_index_path);
				return ;
			}
			struct stat index_stat;
			stat(final_index_path.c_str(), &index_stat);
			_file_stream.open(final_index_path.c_str(), std::ios::in | std::ios::binary);
			{
				if (!_file_stream.is_open())
				{
					_handler_error_code = FORBIDDEN;
					return;
				}
			}
			_httpResponse.buildNormalHeaders(index_stat.st_size, final_index_path);
		}
		else
		{
			if (_location->isAutoindex())
			{
				std::string autoindex_body = generateAutoindex(physical_path, uri);
				if (autoindex_body.empty())
				{
					_handler_error_code = FORBIDDEN;
					return;
				}
				_httpResponse.buildAutoIndexResponse(autoindex_body);
			}
			else
			{
				_handler_error_code = FORBIDDEN;
			}
		}
	}
	else if (S_ISREG(file_stat.st_mode))
	{
		if (isCgiRequest(physical_path, info))
		{
			handleCgiMethod(physical_path);
			return ;
		}
		_file_stream.open(physical_path.c_str(), std::ios::in | std::ios::binary);
		if (!_file_stream.is_open())
		{
			_handler_error_code = FORBIDDEN;
			return;
		}
		_httpResponse.buildNormalHeaders(file_stat.st_size, physical_path);
	}
	else
	{
		_handler_error_code = FORBIDDEN;
		return;
	}
}

std::string	RequestHandler::generateAutoindex(const std::string& physical_path, const std::string& uri)
{
	std::string html = "";
	html += "<html><head><title>Index of " + uri + "</title></head><body>";
	html += "<h1>Index of " + uri + "</h1><hr><pre>";
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(physical_path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string filename = ent->d_name;
			if (filename == ".")
			continue;
			std::string full_path = physical_path;
			if (full_path.length() > 0 && full_path[full_path.length() - 1] != '/')
			{
				full_path += "/";
			}
			full_path += filename;
			
			struct stat file_stat;
			std::string date_str = "-";
			std::string size_str = "-";
			
			if (stat(full_path.c_str(), &file_stat) == 0)
			{
				char time_buf[100];
				struct tm *tm_info = localtime(&file_stat.st_mtime);
				strftime(time_buf, sizeof(time_buf), "%d-%b-%Y %H:%M", tm_info);
				date_str = time_buf;
				if (S_ISDIR(file_stat.st_mode))
				{
					filename += "/";
				}
				else
				{
					std::stringstream ss;
					ss << file_stat.st_size;
					size_str = ss.str();
				}
			}
			
			std::string href_uri = uri;
			if (href_uri.length() > 0 && href_uri[href_uri.length() - 1] != '/')
			{
				href_uri += "/";
			}
			html += "<a href=\"" + href_uri + filename + "\">" + filename + "</a>\t\t";
			html += date_str + "\t\t" + size_str + "\n";
		}
		closedir(dir);
	}
	else
	{
		html.clear();
		return (html);
	}
	html += "</pre><hr></body></html>";
	return (html);
}

const Location*	RequestHandler::matchLocation(const std::string& request_uri, const std::vector<Location>& location)
{
	for (size_t i = 0; i < location.size(); ++i)
	{
		if (request_uri.find(location[i].getPrefix()) == 0)
		{
			return &location[i];
		}
	}
	return (NULL);
}

void	RequestHandler::reset(void)
{
	_req_state = REQ_READING_HEADERS;
	_res_state = RES_HEADERS;
	_handler_error_code = NONE;
	_should_close_connection = false;
	_location = NULL;
	_httpRequest.reset();
	_httpResponse.reset();
	if (_file_stream.is_open())
	{
		_file_stream.close();
	}
	_file_stream.clear();
	_redirect_target.clear();
	_cgi_process_pid = -1;
	_cgi_start_time = 0;
	if (_cgi_handler)
	{
		delete _cgi_handler;
		_cgi_handler = NULL;
	}
}