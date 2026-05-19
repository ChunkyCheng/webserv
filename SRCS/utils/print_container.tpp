#include <iostream>
#include <string>

template<typename T>
typename enable_if<is_container<T>::value, std::ostream&>::type
operator<<(std::ostream& os, const T& container)
{
	typename T::const_iterator	it;

	os << "[";
	it = container.begin();
	while (container.size())
	{
		os << *it;
		++it;
		if (it == container.end())
			break ;
		os << ", ";
	}
	os << "]";
	return (os);	
}

template<typename T1, typename T2>
std::ostream&	operator<<(std::ostream& os, const std::pair<T1, T2>& pair)
{
	os << pair.first;
	os << ":";
	os << pair.second;
	return (os);
}
