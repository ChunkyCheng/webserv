/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:30:41 by yelu              #+#    #+#             */
/*   Updated: 2026/03/06 16:21:59 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
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
	private:
		// RequestState _state;
		std::string _method; // First line of the request, e.g., "GET", "POST", etc.
		std::string _path; // The path requested, e.g., "/index.html"
		std::string _version; // The HTTP version, e.g., "HTTP/1.1"
		std::map<std::string, std::string> _headers; // Key-value pairs of headers, e.g., "Host: example.com"
		std::string _body; // After the empty line \r\n\r\n, whatever is left is the Body.

	public:
		void	parse(const char *rawText);

		std::string getMethod() const;
		std::string getPath() const;
		std::string getVersion() const;
		std::map<std::string, std::string> getHeaders() const;
};
