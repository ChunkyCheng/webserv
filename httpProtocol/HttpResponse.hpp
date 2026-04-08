/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:44:49 by yelu              #+#    #+#             */
/*   Updated: 2026/04/09 01:34:08 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <iostream>
#include <map>
#include "HttpStatus.hpp"

class HttpRequest;

class HttpResponse
{
	private:
		HttpResponse(const HttpResponse& other);
		HttpResponse& operator=(const HttpResponse& other);

		std::string	_version;
		HttpStatus	_status_code;
		std::string	_reason_phrase;

		std::map<std::string, std::string>	_headers;
		std::string							_body;

	public:
		HttpResponse();
		~HttpResponse();

		void	setStatusCode(HttpStatus status_code);
		void	setHeader(const std::string& key, const std::string& value);

};

#endif