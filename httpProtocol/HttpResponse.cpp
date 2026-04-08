/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:44:52 by yelu              #+#    #+#             */
/*   Updated: 2026/04/09 00:59:08 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
	: _version("HTTP/1.1"),
	_status_code(NONE)
{}

HttpResponse::~HttpResponse() {}

void	HttpResponse::setStatusCode(HttpStatus status_code)
{
	_status_code = status_code;
	switch(status_code)
	{
		case OK:
			_reason_phrase = "OK";
			break;
		case NO_CONTENT:
			_reason_phrase = "No Content";
			break;
		case BAD_REQUEST:
			_reason_phrase = "Bad Request";
			break;
		case NOT_FOUND:
			_reason_phrase = "Not Found";
			break;
		case INTERNAL_SERVER_ERROR:
			_reason_phrase = "Internal Server Error";
		case NOT_IMPLEMENTED:
			_reason_phrase = "Not Implemented";
			break;
		default:
			_reason_phrase = "Unknown";
			break;
	}
}

void	HttpResponse::setHeader(const std::string& key, const std::string& value)
{
	_headers[key] = value;
}