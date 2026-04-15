#include "RequestHandler.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>

RequestHandler::RequestHandler(Server& server,
std::string& req_buff, std::string& res_buff)
	: _server(server),
	_request_buff(req_buff),
	_response_buff(res_buff),
	_httpRequest(),
	_req_state(REQ_READING_HEADERS),
	_res_state(RES_HEADERS),
	_matchedLocation(NULL),
	_isFileOpen(false)
{
}

RequestHandler::~RequestHandler(void)
{
}

// void	RequestHandler::reset(void)
// {
// 	_req_state = REQ_READING_HEADERS;
// 	_res_state = RES_HEADERS;
// 	_httpRequest.reset();
// 	_httpResponse.reset();
// 	if (file_stream.is_open()) // <-- close any file stream that is left open, put in the std::ifstream  member variable
// 		_file_stream.close();
// }

bool	RequestHandler::checkRequestComplete(void) const
{
	return (_req_state == REQ_COMPLETE || _req_state == REQ_ERROR);
}

void	RequestHandler::continueBuildResponse(void)
{
	if (_res_state == RES_BODY)
	{
		if (_isFileOpen)
		{
			char chunk[8192];
			_fileInStream.read(chunk, sizeof(chunk));
			std::streamsize bytesRead = _fileInStream.gcount();
			if (bytesRead > 0)
			{
				_response_buff.append(chunk, bytesRead);
			}
			if (_fileInStream.eof() || _fileInStream.fail())
			{
				_fileInStream.close();
				_isFileOpen = false;
				_res_state = RES_FINISHED;
			}
		}
	}
	else
	{
		_response_buff += _httpResponse.getBody();
		_res_state = RES_FINISHED;
	}
}

bool	RequestHandler::checkResponseComplete(void) const
{
	return (_res_state == RES_FINISHED);
}

void	RequestHandler::processReqData(void)
{
	bool	keep_processing;

	keep_processing = true;
	while (keep_processing)
	{
		switch (_req_state)
		{
			case REQ_READING_HEADERS:
				if (_httpRequest.parseHeaders(_request_buff))
				{
					_req_state = _httpRequest.hasBody() ? REQ_READING_BODY : REQ_COMPLETE;
				}
				else
				{
					if (_httpRequest.hasError())
						_req_state = REQ_ERROR;
					else
						keep_processing = false;
				}
				break;

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
				// if (isFatalError(_httpRequest.getError())) If is error, clear off buffer?
				// {
				// 	_request_buff.clear();
				// }
				keep_processing = false;
				break;
		}
	}
}

void    RequestHandler::buildResponseData(void)
{
	std::cout << "\n--- PHASE 3: FILE STREAMING ---" << std::endl;

	std::string path = _httpRequest.getPath();
	if (!path.empty())
		_matchedLocation = matchLocation(path, _server.getLocations());

	// Error Intercept
	HttpStatus req_error = _httpRequest.getError();
	if (req_error != NONE)
	{
		_httpResponse.buildErrorPage(req_error, "");
		_response_buff = _httpResponse.getFormattedHeaders() + _httpResponse.getBody();
		_res_state = RES_FINISHED;
		return;
	}

	if (_matchedLocation == NULL)
	{
		_httpResponse.buildErrorPage(NOT_FOUND, "");
		_response_buff = _httpResponse.getFormattedHeaders() + _httpResponse.getBody();
		_res_state = RES_FINISHED;
		return;
	}

	std::string method = _httpRequest.getMethod();
	std::string physical_path = _matchedLocation->getRoot() + path;

	if (method == "GET")
	{
		struct stat path_info;
		
		// 1. Check if path exists
		if (stat(physical_path.c_str(), &path_info) == 0)
		{
			// 2. Check if it's a directory
			if (S_ISDIR(path_info.st_mode))
			{
				if (physical_path[physical_path.length() - 1] != '/')
					physical_path += '/';
				
				std::vector<std::string> index_files = _matchedLocation->getIndex();
				if (!index_files.empty())
				{
					physical_path += index_files[0];
				}
				else
				{
					_httpResponse.buildErrorPage(NOT_FOUND, "");
					_response_buff = _httpResponse.getFormattedHeaders() + _httpResponse.getBody();
					_res_state = RES_FINISHED;
					return;
				}
			}
		}
		else
		{
			_httpResponse.buildErrorPage(NOT_FOUND, "");
			_response_buff = _httpResponse.getFormattedHeaders() + _httpResponse.getBody();
			_res_state = RES_FINISHED;
			return;
		}

		// 3. SAFE FILE OPENING ON THE PRIVATE MEMBER
		std::cout << "Attempting to open file: " << physical_path << std::endl;
		_fileInStream.open(physical_path.c_str(), std::ios::in | std::ios::binary);

		if (_fileInStream.is_open())
		{
			std::cout << "SUCCESS! File stream is open!" << std::endl;
			_isFileOpen = true;
			_httpResponse.setStatusCode(OK);

			// Add basic headers so the browser knows what to do
			_httpResponse.addHeader("Content-Type", "text/html");
			_httpResponse.addHeader("Content-Length", _httpResponse.sizeToString(static_cast<size_t>(stat(physical_path.c_str(), &path_info) == 0 ? path_info.st_size : 0)));
			_response_buff = _httpResponse.getFormattedHeaders();
			_res_state = RES_BODY; // Drop into RES_BODY so continueBuildResponse takes over!
		}
		else
		{
			std::cout << "FAILED! File could not be opened." << std::endl;
			_httpResponse.buildErrorPage(NOT_FOUND, "");
			_response_buff = _httpResponse.getFormattedHeaders() + _httpResponse.getBody();
			_res_state = RES_FINISHED;
		}
	}
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
