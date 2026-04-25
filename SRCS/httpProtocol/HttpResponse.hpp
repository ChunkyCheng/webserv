/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:44:49 by yelu              #+#    #+#             */
/*   Updated: 2026/04/25 22:20:58 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <iostream>
#include <map>
#include "HttpStatus.hpp"
#include <string>
#include <fstream>

class HttpRequest;

class HttpResponse
{
	private:
		HttpResponse(const HttpResponse& other);
		HttpResponse& operator=(const HttpResponse& other);

		std::string								_version;
		HttpStatus								_status_code;
		std::string								_reason_phrase;
		std::multimap<std::string, std::string>	_headers;
		std::string								_body;

	public:
		HttpResponse();
		~HttpResponse();

		void		setStatusCode(HttpStatus status_code);
		void		overwriteHeader(const std::string& key, const std::string& value);
		void		addHeader(const std::string& key, const std::string& value);
		void		appendHeader(const std::string& key, const std::string& value);

		const std::string&	getReasonPhrase() const;
		std::string			getFormattedHeaders() const;
		const std::string&	getBody() const;
		std::string			getMimeType(const std::string& path);

		void				setBody(const std::string& body);

		void				buildErrorPage(HttpStatus error_code, const std::string& body_content);
		void				buildNormalHeaders(std::streamsize file_size, const std::string& physical_path);
		void				buildRedirectHeaders(const std::string& target, HttpStatus code);
		std::string			sizeToString(size_t number) const;
		void				reset();
};

#endif