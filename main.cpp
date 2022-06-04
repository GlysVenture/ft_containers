
#include <iostream>
#include "vector.hpp"
#include <vector>
#include "iterator.hpp"

int main()
{
	ft::vector<std::string> my_vec;
	std::vector<std::string> a_vec;

	my_vec.push_back("1");
	my_vec.push_back("2");
	my_vec.push_back("3");
	my_vec.push_back("4");

	a_vec.insert(a_vec.begin(), &*my_vec.begin(), &*my_vec.end());

	my_vec.insert(my_vec.begin() + 2, 4, "-");
	my_vec.insert(my_vec.begin() + 4, ft::vector<std::string>::iterator(&*a_vec.begin()), ft::vector<std::string>::iterator(&*a_vec.end()));

	for (ft::vector<std::string>::iterator it = my_vec.begin();  it != my_vec.end() ; it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout << "---" << std::endl;

	my_vec.erase(my_vec.begin());
	my_vec.erase(my_vec.begin() + 4, my_vec.end() - 3);

	for (ft::vector<std::string>::iterator it = my_vec.begin();  it != my_vec.end() ; it++)
	{
		std::cout << *it << std::endl;
	}

	return 0;
}
