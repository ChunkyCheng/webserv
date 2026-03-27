/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:37:12 by yelu              #+#    #+#             */
/*   Updated: 2026/03/27 16:48:26 by yelu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

//substr(size_t pos, size_t len)
// we plus 1 because index starts with 0, and we don't want to miss the last valid character
std::string utils_trim(const std::string& str)
{
	size_t	first;
	size_t	last;

	first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos)
		return "";
	last = str.find_last_not_of(" \t\r\n");
	return (str.substr(first, (last - first + 1)));
}
