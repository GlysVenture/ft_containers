//
// Created by tkondrac on 6/4/22.
//

#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP

#include <functional>
#include <memory>
#include "utility.hpp"
#include "b_tree.hpp"
#include "algorithm.hpp"

namespace ft{

	template<
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map
	{
	public:
		class value_compare;
	private:
		typedef BTree<ft::pair<const Key, T>, Allocator, value_compare> btree;

	public:
		typedef Key									key_type;
		typedef T									mapped_type;
		typedef ft::pair<const Key, T>				value_type;
		typedef std::size_t							size_type;
		typedef long 								difference_type;
		typedef Compare								key_compare;
		typedef Allocator							allocator_type;
		typedef typename btree::reference			reference;
		typedef typename btree::const_reference		const_reference;
		typedef typename btree::pointer				pointer;
		typedef typename btree::const_pointer		const_pointer;
		typedef typename btree::iterator 			iterator;
		typedef typename btree::const_iterator 		const_iterator;
		typedef ft::reverse_iterator<iterator>		reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		///Compare class
		typedef Compare elem_compare;
	class value_compare : std::binary_function< value_type, value_type, bool >
		{
			friend class map;
		protected:
			Compare comp;
			explicit value_compare(Compare c) : comp(c) {}

		public:
			typedef bool result_type;
			typedef value_type first_argument_type;
			typedef value_type second_argument_type;
			bool operator()(const value_type &x, const value_type &y) const
			{
				return comp(x.first, y.first);
			}
		};

	private:
		value_compare		_comparator;
		btree				_tree;

	public:
		map( const Compare& comp = Compare(),
					  const Allocator& alloc = Allocator() ):
					  _comparator(comp), _tree( _comparator, alloc)
	  	{  }

		template< class InputIt >
		map( InputIt first, InputIt last,
			 const Compare& comp = Compare(),
			 const Allocator& alloc = Allocator() ):
			 _comparator(comp), _tree( _comparator, alloc){
			while (first != last){
				_tree.add(*first);
				first++;
			}
		}
		map( const map& other ): _comparator(other._comparator.comp), _tree(other._tree) {  }
		~map() {  }

		map& operator=( const map& other ) {
			_comparator = other._comparator;
			_tree = other._tree;
			return *this;
		}

		allocator_type get_allocator() const{
			return _tree.get_alloc();
		}

		///insert()
		ft::pair<iterator, bool> insert( const value_type& value ){
			size_type s = _tree.size();
			iterator it = _tree.add(value);
			if (s < _tree.size())
				return ft::make_pair(it, true);
			return ft::make_pair(it, false);
		}

		iterator insert( iterator hint, const value_type& value ){
			(void) hint;
			return insert(value).first;
		}

		template< class InputIt >
		void insert( InputIt first, InputIt last ){
			while (first != last){
				_tree.add(*first);
				first++;
			}
		}

		///at()
		T& at( const Key& key ){
			value_type temp = ft::make_pair(key, mapped_type());
			iterator it = _tree.find(temp);
			if (it == --_tree.end())
				throw std::out_of_range("nonexistent key");
			return (*it).second;
		}

		const T& at( const Key& key ) const{
			value_type temp = ft::make_pair(key, mapped_type());
			iterator it = _tree.find(temp);
			if (it == --_tree.end())
				throw std::out_of_range("nonexistent key");
			return (*it).second;
		}

		T& operator[]( const Key& key ){
			return this->insert(ft::make_pair(key,mapped_type())).first->second;
		}

		//Iterators
		iterator begin(){
			return _tree.begin();
		}

		const_iterator begin() const{
			return _tree.begin();
		}

		iterator end(){
			return _tree.end();
		}

		const_iterator end() const{
			return _tree.end();
		}

		reverse_iterator rbegin(){
			return reverse_iterator(_tree.end());
		}

		const_reverse_iterator rbegin() const{
			return const_reverse_iterator(_tree.end());
		}

		reverse_iterator rend(){
			return reverse_iterator(_tree.begin());
		}

		const_reverse_iterator rend() const{
			return const_reverse_iterator(_tree.begin());
		}

		//size thingys
		bool empty() const{
			if (_tree.size())
				return false;
			return true;
		}

		size_type size() const{
			return _tree.size();
		}

		size_type max_size() const{
			return _tree.get_alloc().max_size();
		}

		void clear(){
			_tree.clear();
		}

		void erase( iterator pos ){
			_tree.remove(pos);
		}

		void erase( iterator first, iterator last ){
			_tree.remove(first, last);
		}

		size_type erase( const Key& key ){
			return _tree.remove(ft::make_pair(key, mapped_type()));
		}

		void swap( map& other ){
			value_compare temp_comp = _comparator;

			_comparator = other._comparator;

			other._comparator = temp_comp;
			_tree.swap(other._tree);
		}

		//Search funcs
		size_type count( const Key& key ) const{
			if (_tree.find(ft::make_pair(key, mapped_type())) == _tree.end())
				return 0;
			return 1;
		}

		iterator find( const Key& key ){
			return _tree.find(ft::make_pair(key, mapped_type()));
		}

		const_iterator find( const Key& key ) const{
			return _tree.find(ft::make_pair(key, mapped_type()));
		}

		ft::pair<iterator,iterator> equal_range( const Key& key ){
			return ft::make_pair(lower_bound(key), upper_bound(key));
		}

		ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const{
			return ft::make_pair(lower_bound(key), upper_bound(key));
		}

		iterator lower_bound( const Key& key ){
			return _tree.find_not_less(ft::make_pair(key, mapped_type()));
		}

		const_iterator lower_bound( const Key& key ) const{
			return _tree.find_not_less(ft::make_pair(key, mapped_type()));
		}

		iterator upper_bound( const Key& key ){
			iterator 	it = _tree.find_not_less(ft::make_pair(key, mapped_type()));
			if ((*it).first == key)
				it++;
			return it;
		}

		const_iterator upper_bound( const Key& key ) const{
			const_iterator 	it = _tree.find_not_less(ft::make_pair(key, mapped_type()));
			if ((*it).first == key)
				it++;
			return const_iterator(it);
		}

		//Observers?
		key_compare key_comp() const{
			return _comparator.comp;
		}

		map::value_compare value_comp() const{
			return _comparator;
		}

	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator<( const map<Key,T,Compare,Alloc>& lhs,
					 const map<Key,T,Compare,Alloc>& rhs){
		return ft::lexicographical_compare
			(lhs.begin(), lhs.end(),
			 rhs.begin(), rhs.end());
	}

	template< class Key, class T, class Compare, class Alloc >
	bool operator>( const map<Key,T,Compare,Alloc>& lhs,
					const map<Key,T,Compare,Alloc>& rhs ){
		return rhs < lhs;
	}

	template< class Key, class T, class Compare, class Alloc >
	bool operator==( const map<Key,T,Compare,Alloc>& lhs,
					const map<Key,T,Compare,Alloc>& rhs ){
		if (!(lhs < rhs) && !(rhs < lhs))
			return true;
		return false;
	}

	template< class Key, class T, class Compare, class Alloc >
	bool operator!=( const map<Key,T,Compare,Alloc>& lhs,
					const map<Key,T,Compare,Alloc>& rhs ){
		if (!(lhs < rhs) && !(rhs < lhs))
			return false;
		return true;
	}
	template< class Key, class T, class Compare, class Alloc >
	bool operator>=( const map<Key,T,Compare,Alloc>& lhs,
					const map<Key,T,Compare,Alloc>& rhs ){
		return !(lhs < rhs);
	}
	template< class Key, class T, class Compare, class Alloc >
	bool operator<=( const map<Key,T,Compare,Alloc>& lhs,
					const map<Key,T,Compare,Alloc>& rhs ){
		return !(lhs > rhs);
	}

}

#endif //FT_CONTAINERS_MAP_HPP
