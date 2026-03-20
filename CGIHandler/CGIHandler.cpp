#include "CGIHandler.hpp"

CGIHandler::CGIHandler(HttpRequest& request, std::string& interpreter, std::string& script) :
_request(request), _interpreterPath(interpreter), _scriptPath(script) {}

CGIHandler::~CGIHandler() {}