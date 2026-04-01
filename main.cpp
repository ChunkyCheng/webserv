#include <iostream>

void	parseHeaders()
{
	std::string = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/html\r\nConnection: close\r\n\r\n";
	size_t first_relevant = req_buff.find_first_not_of("\r\n");
	if (first_relevant == std::string::npos)
	{
		req_buff.clear();
		return (false);
	}
	if (first_relevant > 0)
		req_buff.erase(0, first_relevant);
	size_t boundary = req_buff.find("\r\n\r\n");
	if (boundary == std::string::npos)
		return (false);
	std::string raw_headers = req_buff.substr(0, boundary + 4);

}

