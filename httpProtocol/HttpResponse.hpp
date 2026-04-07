/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 14:44:49 by yelu              #+#    #+#             */
/*   Updated: 2026/04/07 19:41:43 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <iostream>
#include <map>
#include "HttpResponse.hpp"

class HttpResponse
{
	private:
		HttpResponse(const HttpResponse& other);
		HttpResponse& operator=(const HttpResponse& other);

		std::string	_version;
		int			_status_code;
		std::string	_reason_phrase;

		std::map<std::string, std::string>	_headers;
		std::string							_body;

	public:
		HttpResponse();
		~HttpResponse();

		void	prepareResponse();


};

#endif