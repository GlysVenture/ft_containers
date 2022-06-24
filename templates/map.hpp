//
// Created by tkondrac on 6/4/22.
//

#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP

#include <functional>
#include <memory>
#include "utility.hpp"
#include "b_tree.hpp"

namespace ft{

	template<
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map
	{
	private:
		size_t	_size;

	public:
		typedef Key									key_type;
		typedef T									mapped_type;
		typedef ft::pair<const Key, T>				value_type;
		typedef std::size_t							size_type;
		typedef long 								difference_type;
		typedef Compare								key_compare;
		typedef Allocator							allocator_type;
		typedef value_type &						reference;
		typedef const value_type &					const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;
		typedef;
	};

}

#endif //FT_CONTAINERS_MAP_HPP
