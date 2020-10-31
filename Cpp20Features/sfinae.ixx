#include <type_traits>
export module sfinae;

/**********************************************************************************************/
/* C++11
*/
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
sumCpp11(T v) { return v; }

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
sumCpp11(T v) { return v + 0.5; }

export 
template <typename T, typename ...Args>
typename std::enable_if<std::is_integral<T>::value, T>::type
sumCpp11(T first, Args... args)
{
	return first + sumCpp11(args...);
}

/**********************************************************************************************/
/* C++17
*/
template<typename T>
typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, T>
sumCpp17(T v)
{ 
	if constexpr (std::is_integral_v<T>) return v;
	else return v + 0.5;
}

export
template<typename T, typename ...Args>
typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, T>
sumCpp17(T first, Args ...args)
{
	return first + sumCpp17(args...);
}

export
template<typename T, 
	typename ...Args, 
	typename Valid = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, T>>
auto sumCpp17Fold(T first, Args ...args)
{
	return (first + ... + args);
}

/**********************************************************************************************/
/* C++20
*/
template<typename T>
concept Real = std::is_floating_point_v<T> || std::is_integral_v<T>;

export
template<Real T, typename ...Args>
auto sumCpp20(T first, Args ...args)
{
	return (first + ... + args);
}