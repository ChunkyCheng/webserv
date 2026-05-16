#ifndef COOKIE_HPP
#define COOKIE_HPP

#include <string>
#include <map>
#include <algorithm>

struct Cookie
{
	std::string name;
	std::string value;
	std::string path;
	std::string domain;
	std::string expires; // raw string, parsing optional
	bool httpOnly;
	bool secure;

	Cookie(): path(""), domain(""), expires(""), httpOnly(false), secure(false) {}

	static Cookie parseSetCookie(const std::string& header)
	{
		Cookie c;
		// Split on ';'
		size_t pos = 0;
		bool first = true;
		while (pos < header.size())
		{
			size_t sep = header.find(';', pos);
			std::string token = header.substr(pos, (sep == std::string::npos) ? std::string::npos : sep - pos);
			// trim
			size_t s = token.find_first_not_of(" \t");
			if (s == std::string::npos) { if (sep==std::string::npos) break; pos = sep+1; continue; }
			size_t e = token.find_last_not_of(" \t");
			token = token.substr(s, e - s + 1);
			if (first)
			{
				first = false;
				size_t eq = token.find('=');
				if (eq != std::string::npos)
				{
					c.name = token.substr(0, eq);
					c.value = token.substr(eq + 1);
				}
			}
			else
			{
				// attribute
				size_t eq = token.find('=');
				std::string attr = token;
				for (size_t i=0;i<attr.size();++i) attr[i] = std::tolower(attr[i]);
				if (eq == std::string::npos)
				{
					if (attr == "httponly") c.httpOnly = true;
					else if (attr == "secure") c.secure = true;
				}
				else
				{
					std::string key = token.substr(0, eq);
					std::string val = token.substr(eq + 1);
					for (size_t i=0;i<key.size();++i) key[i] = std::tolower(key[i]);
					if (key == "path") c.path = val;
					else if (key == "domain") c.domain = val;
					else if (key == "expires") c.expires = val;
					else if (key == "max-age") c.expires = val; // store max-age in expires for simplicity
				}
			}
			if (sep == std::string::npos) break;
			pos = sep + 1;
		}
		return c;
	}
};

#endif
