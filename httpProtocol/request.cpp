/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:29 by yelu              #+#    #+#             */
/*   Updated: 2026/03/04 16:30:35 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 17:33:46 by yelu              #+#    #+#             */
/*   Updated: 2026/03/03 21:47:17 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void HttpRequest::parse(std::string rawText)
{
	std::stringstream ss(rawText);
	std::string line;

	std::getline(ss, line);
}

// void HttpRequest::feed(const char* buffer, int size) {
//     _rawStorage.append(buffer, size); // Add the new "gulp" to the bucket

//     if (_state == READING_HEADERS) {
//         if (_rawStorage.find("\r\n\r\n") != std::string::npos) {
//             parseHeaders(); // This function will find Content-Length
//             _state = READING_BODY;
//         }
//     }

//     if (_state == READING_BODY) {
//         // Check if current body size matches Content-Length header
//         if (_body.size() >= _expectedBodySize) {
//             _state = FINISHED;
//         }
//     }
// }

// Received body length shorter
// int expectedSize = atoi(headers["Content-Length"].c_str());
// if (_body.size() >= expectedSize) {
//     _isReady = true;
// }
