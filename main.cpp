#include <iostream>

#include "vector.hpp"
#include <vector>

int main()
{
	ft::vector<std::string> my_vec;
	std::vector<std::string> a_vec;

	std::cout << "Hello, World! " << std::endl;
	return 0;
}

template<class T>
void destroy(
	T*,
	typename std::enable_if<
		std::is_trivially_destructible<T>::value
	>::type* = 0
){
	std::cout << "destroying trivially destructible T\n";
}
