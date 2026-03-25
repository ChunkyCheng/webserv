#include "CGIHandler.hpp"

int	main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	HttpRequest req;

	req.parse("GET /test.py HTTP/1.1\r\n\r\n");
	CGIHandler handler(req, "/bin/python3", "scripts/test.py");
	HttpResponse res = handler.execute();
	std::cout << res.body << std::endl;
	return (0);
}
