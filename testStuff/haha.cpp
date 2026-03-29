#include <iostream>
#include <string>
#include <cstdlib>

// Mocking your utils_trim
std::string utils_trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Mocking your RequestHandler class structure
class RequestHandler {
public:
    std::string _request_buff;
    bool checkRequestComplete(void) const;
};

bool RequestHandler::checkRequestComplete(void) const {
    size_t header_end = _request_buff.find("\r\n\r\n");
    if (header_end == std::string::npos)
        return (false);

    if (_request_buff.compare(0, 4, "POST") == 0) {
        size_t pos = _request_buff.find("Content-Length: ");
        if (pos != std::string::npos && pos < header_end) {
            size_t val_start = pos + 16;
            size_t val_end = _request_buff.find("\r\n", val_start);
            std::string raw_val = _request_buff.substr(val_start, val_end - val_start);
            std::string clean_val = utils_trim(raw_val);
			std::cout << clean_val << "\n";
            long content_length = std::atol(clean_val.c_str());

            size_t total_header_size = header_end + 4;
            size_t current_body_size = _request_buff.size() - total_header_size;
            return (current_body_size >= (size_t)content_length);
        }

        if (_request_buff.find("Transfer-Encoding: chunked") != std::string::npos) {
            return (_request_buff.find("0\r\n\r\n") != std::string::npos);
        }
    }
    return (true);
}

void print_test(const std::string& label, bool result) {
    std::cout << "[" << (result ? "PASS" : "FAIL") << "] " << label << std::endl;
}

int main() {
    RequestHandler rh;

    // Test 1: Incomplete Header
    rh._request_buff = "POST /index.html HTTP/1.1\r\nHost: localhost";
    print_test("Incomplete Header (should be false)", rh.checkRequestComplete() == false);

    // Test 2: POST with Content-Length (Incomplete Body)
    rh._request_buff = "POST / HTTP/1.1\r\nContent-Length: 10\r\n\r\n12345";
    print_test("POST Content-Length Incomplete (should be false)", rh.checkRequestComplete() == false);

    // Test 3: POST with Content-Length (Complete Body)
    rh._request_buff += "67890";
    print_test("POST Content-Length Complete (should be true)", rh.checkRequestComplete() == true);

    // Test 4: POST Chunked (Incomplete)
    rh._request_buff = "POST / HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n";
    print_test("POST Chunked Incomplete (should be false)", rh.checkRequestComplete() == false);

    // Test 5: POST Chunked (Complete with 0\r\n\r\n)
    rh._request_buff += "0\r\n\r\n";
    print_test("POST Chunked Complete (should be true)", rh.checkRequestComplete() == true);

    // Test 6: Basic GET (Immediate true after header)
    rh._request_buff = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
    print_test("GET Request Complete (should be true)", rh.checkRequestComplete() == true);

    return 0;
}
