#include "CGIHandler.hpp"

int	main(int argc, char *argv[])
{
	(void)argc;
	HttpRequest req;

	req.parse("GET /test.py HTTP/1.1\r\n\r\n");
	CGIHandler handler(req, "/bin/python3", argv[1]);
	HttpResponse res = handler.execute();
	std::cout << "Status: " << res.statusCode << std::endl;
	std::cout << "--- Headers ---" << std::endl;
	std::map<std::string, std::string>::iterator it;
	for (it = res.headers.begin(); it != res.headers.end(); it++) {
		std::cout << it->first << ": " << it->second << std::endl;
	}
	std::cout << "--- Body ---" << std::endl;
	std::cout << res.body << std::endl;
	return (0);
}
