//
// Created by tkondrac on 6/4/22.
//

#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP

#include <functional>
#include <memory>
#include "utility.hpp"

namespace ft{

	template<
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map
	{
	private:
	public:
	};

}

#endif //FT_CONTAINERS_MAP_HPP
