/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:41 by yelu              #+#    #+#             */
/*   Updated: 2026/03/04 16:31:24 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 16:22:46 by yelu              #+#    #+#             */
/*   Updated: 2026/03/03 22:05:45 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <map>

enum RequestState {
	READING_HEADERS,
	READING_BODY,
	FINISHED,
	ERROR
};

class HttpRequest
{
	RequestState _state;
	std::string method; // First line of the request, e.g., "GET", "POST", etc.
	std::string path; // The path requested, e.g., "/index.html"
	std::string version; // The HTTP version, e.g., "HTTP/1.1"
	std::map<std::string, std::string> headers; // Key-value pairs of headers, e.g., "Host: example.com"
	std::string body; // After the empty line \r\n\r\n, whatever is left is the Body.

	void	parse(std::string raw_request);
};
