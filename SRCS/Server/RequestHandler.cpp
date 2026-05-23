#include "RequestHandler.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <ctime>
#include <cerrno>
#include <cstring>


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
	_is_cgi(false)
{
}

RequestHandler::~RequestHandler(void)
{
}

bool	RequestHandler::checkRequestComplete(void) const
{
	return (_req_state == REQ_COMPLETE || _req_state == REQ_ERROR);
}

bool	RequestHandler::checkResponseComplete(void) const
{
	return (_res_state == RES_FINISHED);
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
			{
				size_t max_body_size = _location->getClientMaxBodySize();
				if (_httpRequest.parseBody(_request_buff, max_body_size))
					_req_state = REQ_COMPLETE;
				else
					if (_httpRequest.hasError())
						_req_state = REQ_ERROR;
					else
						keep_processing = false;
				break;

			}

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
	assembleFinalBuffer();
}

void	RequestHandler::continueBuildResponse(void)
{
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
		}
	}
}

// ==============================================================================
// BUILDRESPONSEDATA PRIVATE FUNCTIONS
// ==============================================================================

HttpStatus	RequestHandler::executeMethod()
{
	std::string physical_path = getNormalPagePath();
	std::string method = _httpRequest.getMethod();
	if (method == "GET")
	{
		handleGetMethod(physical_path);
	}
	// else if (method == "POST")
	// {
	// 	handlePostMethod(physical_path);
	// }
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

// void	RequestHandler::handlePostMethod(const std::string& physical_path)
// {
// 	bool upload_enabled = _location->isUploadAllowed();
// 	if (!upload_enabled)
// 	{
// 		_handler_error_code = FORBIDDEN;
// 		return;
// 	}

// 	std::string filename;
// 	std::string filecontent;
// 	std::map<std::string, std::string> headers = _httpRequest.getHeaders();

// 	bool parsed = parseMultipartFile(_httpRequest.getBody(), headers, filename, filecontent);

// 	std::string upload_path;
// 	if (parsed)
// 	{
// 		upload_path = buildSafeUploadPath(physical_path, filename);
// 	}
// 	else
// 	{
// 		upload_path = buildSafeUploadPath(physical_path);
// 		filecontent = _httpRequest.getBody();
// 	}

// 	// Ensure upload directory exists
// 	size_t slash = upload_path.find_last_of('/');
// 	if (slash != std::string::npos)
// 	{
// 		std::string dir = upload_path.substr(0, slash);
// 		if (!ensureDirExists(dir))
// 		{
// 			_handler_error_code = FORBIDDEN;
// 			return;
// 		}
// 	}

// 	std::ofstream file(upload_path.c_str(), std::ios::out | std::ios::binary);
// 	if (!file.is_open())
// 	{
// 		_handler_error_code = FORBIDDEN;
// 		return;
// 	}
// 	file.write(filecontent.c_str(), filecontent.size());
// 	if (file.bad())
// 	{
// 		_handler_error_code = INTERNAL_SERVER_ERROR;
// 	}
// 	else
// 	{
// 		_handler_error_code = CREATED;
// 	}
// 	file.close();
// }

// std::string RequestHandler::buildSafeUploadPath(const std::string& physical_path, const std::string& suggested_filename) const
// {
// 	std::string base_dir = _location->getUploadStore();
// 	if (base_dir.empty())
// 		base_dir = ".";

// 	std::string name;
// 	if (!suggested_filename.empty())
// 		name = sanitizeFilename(suggested_filename);
// 	else
// 	{
// 		size_t pos = physical_path.find_last_of('/');
// 		if (pos == std::string::npos)
// 			name = sanitizeFilename(physical_path);
// 		else
// 			name = sanitizeFilename(physical_path.substr(pos + 1));
// 	}

// 	if (name.empty())
// 	{
// 		// generate fallback name
// 		std::stringstream ss;
// 		ss << "upload_" << std::time(NULL) << "_" << getpid();
// 		name = ss.str();
// 	}

// 	// ensure base_dir ends without '/'
// 	if (!base_dir.empty() && base_dir.back() == '/')
// 		base_dir.erase(base_dir.length() - 1);
// 	return (base_dir + "/" + name);
// }

// bool RequestHandler::ensureDirExists(const std::string& dir_path) const
// {
// 	if (dir_path.empty())
// 		return false;
// 	struct stat st;
// 	if (stat(dir_path.c_str(), &st) == 0)
// 	{
// 		if (S_ISDIR(st.st_mode))
// 			return true;
// 		else
// 			return false;
// 	}
// 	// recursively create parent
// 	size_t pos = dir_path.find_first_of('/');
// 	std::string cur = "";
// 	if (dir_path[0] == '/')
// 		cur = "/";
// 	size_t start = 0;
// 	while (start < dir_path.length())
// 	{
// 		size_t next = dir_path.find('/', start);
// 		std::string part;
// 		if (next == std::string::npos)
// 			part = dir_path.substr(start);
// 		else
// 			part = dir_path.substr(start, next - start);
// 		if (!part.empty())
// 		{
// 			if (!cur.empty() && cur.back() != '/')
// 				cur += "/";
// 			cur += part;
// 		}
// 		struct stat st2;
// 		if (stat(cur.c_str(), &st2) != 0)
// 		{
// 			if (mkdir(cur.c_str(), 0755) != 0 && errno != EEXIST)
// 				return false;
// 		}
// 		if (next == std::string::npos)
// 			break;
// 		start = next + 1;
// 	}
// 	return true;
// }

// std::string RequestHandler::sanitizeFilename(const std::string& filename) const
// {
// 	std::string out;
// 	for (size_t i = 0; i < filename.size(); ++i)
// 	{
// 		char c = filename[i];
// 		if (c == '/' || c == '\\')
// 			continue;
// 		if (c == '.' || c == '_' || c == '-' || std::isalnum(static_cast<unsigned char>(c)))
// 			out.push_back(c);
// 		else
// 			out.push_back('_');
// 		if (out.size() >= 200)
// 			break;
// 	}
// 	// remove leading dots to avoid hidden files or relative paths
// 	while (!out.empty() && out[0] == '.')
// 		out.erase(out.begin());
// 	return out;
// }

// bool RequestHandler::parseMultipartFile(const std::string& body, const std::map<std::string, std::string>& headers, std::string& out_filename, std::string& out_filecontent) const
// {
// 	std::map<std::string, std::string>::const_iterator it = headers.find("content-type");
// 	if (it == headers.end())
// 		return false;
// 	std::string ctype = it->second;
// 	std::string boundary_key = "boundary=";
// 	size_t bpos = ctype.find(boundary_key);
// 	if (bpos == std::string::npos)
// 		return false;
// 	std::string boundary = ctype.substr(bpos + boundary_key.length());
// 	// strip possible quotes
// 	if (!boundary.empty() && boundary.front() == '"' && boundary.back() == '"')
// 	{
// 		boundary = boundary.substr(1, boundary.length() - 2);
// 	}
// 	std::string delim = "--" + boundary;

// 	size_t part_start = body.find(delim);
// 	if (part_start == std::string::npos)
// 		return false;
// 	// move to end of boundary line
// 	size_t line_end = body.find("\r\n", part_start);
// 	if (line_end == std::string::npos)
// 		return false;
// 	size_t hdr_start = line_end + 2;
// 	size_t hdr_end = body.find("\r\n\r\n", hdr_start);
// 	if (hdr_end == std::string::npos)
// 		return false;
// 	std::string part_headers = body.substr(hdr_start, hdr_end - hdr_start);
// 	std::string filename;
// 	// parse part headers
// 	size_t pos = 0;
// 	while (pos < part_headers.size())
// 	{
// 		size_t eol = part_headers.find("\r\n", pos);
// 		std::string line;
// 		if (eol == std::string::npos)
// 		{
// 			line = part_headers.substr(pos);
// 			pos = part_headers.size();
// 		}
// 		else
// 		{
// 			line = part_headers.substr(pos, eol - pos);
// 			pos = eol + 2;
// 		}
// 		// look for Content-Disposition
// 		std::string low = line;
// 		std::transform(low.begin(), low.end(), low.begin(), ::tolower);
// 		if (low.find("content-disposition:") == 0)
// 		{
// 			size_t fname_pos = line.find("filename=");
// 			if (fname_pos != std::string::npos)
// 			{
// 				size_t q = line.find('"', fname_pos);
// 				if (q != std::string::npos)
// 				{
// 					size_t q2 = line.find('"', q + 1);
// 					if (q2 != std::string::npos)
// 						filename = line.substr(q + 1, q2 - q - 1);
// 				}
// 				else
// 				{
// 					// unquoted value until semicolon or end
// 					size_t endpos = line.find(';', fname_pos);
// 					if (endpos == std::string::npos)
// 						endpos = line.length();
// 					filename = line.substr(fname_pos + 9, endpos - (fname_pos + 9));
// 				}
// 			}
// 		}
// 	}
// 	if (filename.empty())
// 		return false;

// 	size_t content_start = hdr_end + 4;
// 	std::string boundary_search = "\r\n" + delim;
// 	size_t content_end = body.find(boundary_search, content_start);
// 	if (content_end == std::string::npos)
// 	{
// 		// try without preceding CRLF
// 		content_end = body.find(delim, content_start);
// 		if (content_end == std::string::npos)
// 			return false;
// 	}
// 	out_filecontent = body.substr(content_start, content_end - content_start);
// 	// strip trailing CRLF if present
// 	if (out_filecontent.size() >= 2 && out_filecontent.substr(out_filecontent.size() - 2) == "\r\n")
// 		out_filecontent.erase(out_filecontent.size() - 2);

// 	out_filename = filename;
// 	return true;
// }

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
	std::string uri = getPathOnly(_httpRequest.getPath());
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
	size_t dot_pos = uri.find_last_of('.');
	if (dot_pos != std::string::npos)
	{
		std::string ext = uri.substr(dot_pos);
		if (isCgiExtension(ext))
		{
			_is_cgi = true;
		}
	}
	return (OK);
}

bool	RequestHandler::isCgiExtension(std::string &ext)
{
	const char* valid_ext[] = {".py", ".php", ".cgi"};
	size_t num_ext = sizeof(valid_ext) / sizeof(valid_ext[0]);
	for (size_t i = 0; i < num_ext; ++i)
	{
		if (ext == valid_ext[i])
		{
			return (true);
		}
	}
	return (false);
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
	std::string uri = getPathOnly(_httpRequest.getPath());
	if (!root.empty() && root[root.length() - 1] == '/' && !uri.empty() && uri[0] == '/')
	{
		root.erase(root.length() - 1);
	}
	std::string full_path = root + uri;
	// std::cout << full_path;
	return (full_path);
}


void RequestHandler::handleDeleteMethod(const std::string& physical_path)
{
	if (access(physical_path.c_str(), F_OK) != 0)
	{
		_handler_error_code = NOT_FOUND;
		return;
	}
	if (access(physical_path.c_str(), W_OK) != 0)
	{
		_handler_error_code = FORBIDDEN;
		return;
	}
	if (unlink(physical_path.c_str()) == 0)
	{
		_handler_error_code = NO_CONTENT;
	}
	else
	{
		_handler_error_code = INTERNAL_SERVER_ERROR;
	}
}

void RequestHandler::handleGetMethod(const std::string& physical_path)
{
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
		std::string path = getPathOnly(uri);
		if (!path.empty() && path[path.length() - 1] != '/')
		{
			_handler_error_code = MOVED_PERMANENTLY;
			std::string query_and_frag = uri.substr(path.length());
			_redirect_target = path + "/" + query_and_frag;
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
				std::string autoindex_body = generateAutoindex(physical_path, path);
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

const Location*	RequestHandler::matchLocation(const std::string& request_uri, const std::vector<Location>& locations)
{
	for (size_t i = 0; i < locations.size(); ++i)
	{
		const std::string& prefix = locations[i].getPrefix();
		if (request_uri.compare(0, prefix.size(), prefix) != 0)
			continue;
		if (request_uri.size() == prefix.size()
			|| prefix[prefix.size() - 1] == '/'
			|| request_uri[prefix.size()] == '/')
		{
			return (&locations[i]);
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
}

bool	RequestHandler::getShouldCloseConnection(void) const
{
	return (_should_close_connection);
}

std::string getPathOnly(const std::string& uri)
{
	size_t pos = uri.find_first_of("?#");
	if (pos == std::string::npos)
		return uri;
	return uri.substr(0, pos);
}
