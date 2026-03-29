/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:29:52 by yelu              #+#    #+#             */
/*   Updated: 2026/03/29 13:14:50 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

int main()
{
	const char *raw_request = "GET /index.html HTTP/1.1\r\nHost: example.com       \r\nUser-Agent: Mozilla/5.0\r\n\r\n";
	HttpRequest request;
	request.parse(raw_request);
	return 0;
}
