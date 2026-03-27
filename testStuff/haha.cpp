/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   haha.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:20:52 by yelu              #+#    #+#             */
/*   Updated: 2026/03/27 16:21:23 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "haha.hpp"

bool	RequestHandler::checkRequestComplete(void) const
{
	size_t	header_end;

	header_end = _request_buff.find("/r/n/r/n");
	// Check if header finished loading, if returns 0, return false
	if (header_end == std::string::npos)
		return (false);
	// Only return true if found the header
	// Find the method, POST usually have body
	if (_request_buff.compare(0, 4, "POST") == 0)
	{
		size_t pos = _request_buff.find("Content-Length: ");
		// Check there is Content-Length
		if (pos != std::string::npos && pos < header_end)
		{
			// Find the number between content length header, and the end \r\n, every header line follows this format
			size_t val_start = pos + 16;
			size_t val_end = _request_buff.find("\r\n", val_start);

			long content_length = std::atol(_request_buff.substr(val_start, val_end - val_start).c_str());
			size_t current_body_size = _request_buff.size() - (header_end + 4);

			// It is possible the same client send another request immediately, so the current body
			// size will be longer than the content length intended, it accounted for the second's request too
			// the most important thing is that it finished fetching the first request, so return true first
				return (current_body_size >= (size_t)content_length);
		}

		// It is possible there is no Content-length, but there is body? 
		// Might be chunked?
		if (_request_buff.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			// Usually chunked request ends with 0\r\n\r\n
			return (_request_buff.find("0\r\n\r\n") != std::string::npos);
		}
	}
	
	// If we didn't find content-length, maybe it's "chunked?"
	if (_request_buff.find("Transfer-Encoding: chunked") != )
	// For GET, DELETE, once /r/n/r/n found, usually means done so return true to signal the start of response processing
	return (true);

	// PROBLEM: What if body is not found? This logic immediately returns true
	// Needed reading: Transfer-encoding: chunked
}
