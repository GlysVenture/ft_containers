
#include <iostream>
#include "vector.hpp"
#include <vector>
#include "iterator.hpp"

int main()
{
	ft::vector<std::string> my_vec;
	std::vector<std::string> a_vec;

	std::cout << "Hello, World! " << std::endl;

	int tab[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	ft::random_access_iterator<int> iter(tab);

	std::cout << "iterators: " << std::endl;
	std::cout << *iter << std::endl;
	std::cout << *(iter + 5) << std::endl;
	std::cout << std::boolalpha << ((iter) > iter) << std::endl;
	std::cout << std::boolalpha << ((iter) >= iter) << std::endl;
	std::cout << iter[1] << std::endl;
	return 0;
}
