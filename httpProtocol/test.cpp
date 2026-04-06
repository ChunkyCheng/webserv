/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:29:52 by yelu              #+#    #+#             */
/*   Updated: 2026/04/07 00:26:10 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

void test_nice_get()
{
    HttpRequest req;
    std::string req_buff = "POST /index.html HTTP/1.1\r\n"
                            "Host: localhost:8080\r\n"
                            // "Content-Length: \r\n"
                            "Transfer-Encoding: chunked\r\n"
                            "\r\n"
                            "Hello World!";

    bool result = req.parseHeaders(req_buff);
    // bool result2 = req.parseBody(req_buff);
    std::cout << "Parse returned: " << (result ? "TRUE" : "FALSE") << std::endl;
    std::cout << "Method: " << req.getMethod() << std::endl;
    std::cout << "Path: " << req.getPath() << std::endl;
    // std::cout << "Body: " << req.getBody() << std::endl;
    std::cout << "Buffer left over (should be empty): '" << req_buff << "'" << std::endl;
}

int main()
{
    test_nice_get();
}