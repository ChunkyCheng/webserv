/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:44:49 by yelu              #+#    #+#             */
/*   Updated: 2026/04/09 16:21:34 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <iostream>
#include <map>
#include "HttpStatus.hpp"
#include <string>

class HttpRequest;

class HttpResponse
{
	private:
		HttpResponse(const HttpResponse& other);
		HttpResponse& operator=(const HttpResponse& other);

		std::string	_version;
		HttpStatus	_status_code;
		std::string	_reason_phrase;

		std::multimap<std::string, std::string>	_headers;
		std::string								_body;

	public:
		HttpResponse();
		~HttpResponse();

		void	setStatusCode(HttpStatus status_code);
		void	overwriteHeader(const std::string& key, const std::string& value);
		void	addHeader(const std::string& key, const std::string& value);
		void	appendHeader(const std::string& key, const std::string& value);


};

#endif