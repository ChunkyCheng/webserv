#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>

template<bool condition, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T>
{
	typedef T	type;
};

template<typename T>
struct is_container
{
	private:
		typedef char yes[1];
		typedef char no[2];

		template<typename C>
		static yes& test(typename C::const_iterator*);

		template<typename>
		static no&	test(...);

	public:
		static const bool	value = (sizeof(test<T>(0)) == sizeof(yes));
};

template<>
struct is_container<std::string>
{
	public:
		static const bool	value = false;
};

//print container
template<typename T>
typename enable_if<is_container<T>::value, std::ostream&>::type
operator<<(std::ostream& os, const T& container);

template<typename T1, typename T2>
std::ostream&	operator<<(std::ostream& os, const std::pair<T1, T2>& pair);

# include "print_container.tpp"

#endif
