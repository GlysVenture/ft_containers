//
// Created by Tadeusz Kondracki on 4/21/22.
//

#ifndef FT_CONTAINERS_VECTOR_HPP
# define FT_CONTAINERS_VECTOR_HPP

#include <cstddef>
#include <cstring>
#include <stdint.h>
#include "utility.hpp"
#include "iterator.hpp"

namespace ft {

	///Vector Template
	//Class Declaration
	template<typename T, class Allocator = std::allocator<T> >
	class vector {
	private:
		size_t _size;
		size_t _capacity;
		T *	arr;
		Allocator alloc;

	public:
		vector();
		explicit vector(const Allocator & alloc);
		vector(const vector<T, Allocator> & inst);
		~vector();

		typedef T										value_type;
		typedef Allocator								allocator_type;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		typedef random_access_iterator<T>				iterator;
		typedef const random_access_iterator<T>			const_iterator;

		vector<T, Allocator> & operator=(const vector<T, Allocator> &rhs);

		//funcs
		void assign(size_type count, const T& value );
		template< class InputIt >
		void assign( InputIt first, InputIt last );
		allocator_type get_allocator() const;

		//access
		reference at( size_type pos );
		const_reference at( size_type pos ) const;
		reference operator[](size_t i);
		const_reference operator[](size_t i) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		T* data();
		const T* data() const;

		//iterators
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		//capacity
		bool empty() const;
		size_type size() const;
		size_type max_size() const;
		void reserve( size_type new_cap );
		size_type capacity() const;

		//modifiers
		void clear();
		iterator insert( iterator pos, const T& value );
		void insert( iterator pos, size_type count, const T& value );
		template< class InputIt >
		void insert( iterator pos, InputIt first, InputIt last );
		iterator erase( iterator pos );
		iterator erase( iterator first, iterator last );
		void push_back( const T& value );
		void pop_back();
		void resize( size_type count, T value = T() );
		void swap( vector& other );

		//operators
	};

	//Function Definitions
	//Constructors ------------------------------------------------------------

	///default constructor
	template<typename T, class Allocator>
	vector<T, Allocator>::vector(): _size(0), _capacity(0)
	{
		arr = alloc.allocate(_capacity);
	}

	///allocator constructor
	template<typename T, class Allocator>
	vector<T, Allocator>::vector(const Allocator &alloc): alloc(alloc), _size(0), _capacity(0)
	{
		arr = alloc.allocate(_capacity);
	}

	///copy constructor
	template<typename T, class Allocator>
	vector<T, Allocator>::vector(const vector<T, Allocator> &inst) : _size(inst._size), _capacity(inst._capacity)
	{
		arr = alloc.allocate(_capacity);
		copy<T, Allocator>(arr, inst._arr(), _size , alloc);
	}

	///default destructor
	template<typename T, class Allocator>
	vector<T, Allocator>::~vector()
	{
		for (size_t i = 0; i < _size; ++i ) {
			alloc.destroy(arr + i);
		}
		alloc.deallocate(arr, _capacity);
	}

	///copy = operator
	template<typename T, class Allocator>
	vector<T, Allocator> & vector<T, Allocator>::operator=(const vector<T, Allocator> &rhs)
	{
		T *temp = alloc.allocate(rhs._capacity);

		copy<T, Allocator>(temp, rhs.arr, rhs._size , alloc);
		this->~vector();
		this->_size = rhs._size;
		this->_capacity = rhs._capacity;
		this->arr = temp;
		return *this;
	}

	//Member functions --------------------------------------------------------

	///assign() with value
	template<typename T, class Allocator>
	void vector<T, Allocator>::assign(vector::size_type count, const T& value){
		for (size_t i = 0; i < _size; ++i ) {
			alloc.destroy(arr[i]);
		}
		this->reserve(count);
		this->_size = count;
		for (size_t i = 0; i < _size; ++i)
		{
			alloc.construct(arr, value);
		}
	}

	///assign() with list
	template<typename T, class Allocator>
	template< class InputIt >
	void vector<T, Allocator>::assign( InputIt first, InputIt last ){
		size_type count = last - first;

		for (size_t i = 0; i < _size; ++i ) {
			alloc.destroy(arr + i);
		}
		this->reserve(count);
		this->_size = count;
		for (size_t i = 0; i < _size; ++i)
		{
			alloc.construct(arr, first++);
		}
	}

	///get_allocator()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const{
		return alloc;
	}

	//Element access ----------------------------------------------------------

	///at()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(vector::size_type pos)
	{
		if (pos >= _size)
			throw std::out_of_range("vector out of bounds access");
		return arr[pos];
	}

	///at() const
	template<typename T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(vector::size_type pos) const
	{
		if (pos >= _size)
			throw std::out_of_range("vector out of bounds access");
		return arr[pos];
	}

	///access [] operator
	template<typename T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](vector::size_type i)
	{
		return arr[i];
	}

	///access [] operator const
	template<typename T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](vector::size_type i) const
	{
		return arr[i];
	}

	///front()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::front()
	{
		return arr[0];
	}

	///front() const
	template<typename T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const
	{
		return arr[0];
	}

	///back()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::back()
	{
		return arr[_size - 1];
	}

	///back() const
	template<typename T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const
	{
		return arr[_size - 1];
	}

	///data()
	template<typename T, class Allocator>
	T * vector<T, Allocator>::data()
	{
		return arr;
	}

	///data() const
	template<typename T, class Allocator>
	const T * vector<T, Allocator>::data() const
	{
		return arr;
	}

	//Iterators ---------------------------------------------------------------

	template<typename T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::begin()
	{
		return vector::iterator(arr);
	}

	template<typename T, class Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const
	{
		return vector::iterator(arr);
	}

	template<typename T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::end()
	{
		return vector::iterator(arr + _size);
	}

	template<typename T, class Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const
	{
		return vector::iterator(arr + _size);
	}

	//todo reverse iterator compatibility funcs

	//Capacity ----------------------------------------------------------------

	///empty()
	template<typename T, class Allocator>
	bool vector<T, Allocator>::empty() const {
		if (_size == 0)
			return true;
		return false;
	}

	///size()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const
	{
		return _size;
	}

	///max_size()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const
	{
		return SIZE_MAX;
	}

	///reserve()
	template<typename T, class Allocator>
	void vector<T, Allocator>::reserve(vector::size_type new_cap)
	{
		if (new_cap > this->max_size())
			throw std::length_error("vector capacity requested is too big");
		if (new_cap > _capacity)
		{
			T * temp = alloc.allocate(new_cap);

			rebuild(temp, arr, _size, alloc);
			alloc.deallocate(arr, _capacity);
			arr = temp;
			_capacity = new_cap;
		}
	}

	///capacity()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const
	{
		return _capacity;
	}

	//Modifers ----------------------------------------------------------------

	template<typename T, class Allocator>
	void vector<T, Allocator>::clear()
	{
		for (size_t i = 0; i < _size; ++i ) {
			alloc.destroy(arr + i);
		}
		_size = 0;
	}

	///insert()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::iterator
	vector<T, Allocator>::insert(vector::iterator pos, const T &value)
	{
		if (pos == this->end())
		{
			this->push_back(value);
			return (this->end() - 1);
		}
		size_t index = pos - this->begin();
		this->reserve(_size + 1);
		move(arr + index + 1, arr + index, _size - index);
		alloc.construct(arr + index, value);
		_size++;
		return vector::iterator(arr + index);
	}

	///insert() with multiple values
	template<typename T, class Allocator>
	void
	vector<T, Allocator>::insert(vector::iterator pos, vector::size_type count,
								 const T &value)
	{
		if (count == 0)
			return;
		if (pos == this->end())
		{
			for (size_type i = 0; i < count; ++i){
				this->push_back(value);
			}
			return;
		}
		size_t index = pos - this->begin();
		this->reserve(_size + count);
		move<T, Allocator>(arr + index + count, arr + index, _size - index, alloc);
		for (size_type i = 0; i < count; ++i){
			alloc.construct(arr + index + i, value);
		}
		_size = _size + count;
	}

	///insert() with iterator range
	template<typename T, class Allocator>
	template<class InputIt>
	void vector<T, Allocator>::insert(vector::iterator pos, InputIt first,
									  InputIt last)
	{
		size_type count = last - first;

		if (count == 0)
			return;
		if (pos == this->end())
		{
			while (first != last){
				this->push_back(*(first++));
			}
			return;
		}
		size_t index = pos - this->begin();
		this->reserve(_size + count);
		move(arr + index + count, arr + index, _size - index, alloc);
		size_type i = 0;
		while (first != last){
			alloc.construct(arr + index + i, *(first++));
			i++;
		}
		_size = _size + count;
	}

	///erase()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::iterator
	vector<T, Allocator>::erase(vector::iterator pos)
	{
		if (this->empty())
			return this->end();
		size_type len = this->end() - pos - 1;
		vector::pointer p = &*pos;
		alloc.destroy(p);
		_size--;
		move(p, p + 1, len, alloc);
		return vector::iterator(p);
	}

	///erase() it range
	template<typename T, class Allocator>
	typename vector<T, Allocator>::iterator
	vector<T, Allocator>::erase(vector::iterator first, vector::iterator last)
	{
		if (this->empty())
			return this->end();
		size_type len = this->end() - last;
		vector::iterator p = last;
		while (first != p){
			p--;
			alloc.destroy(&*p);
			_size--;
		}
		move(&*first, &*last, len, alloc);
		return vector::iterator(p);
	}

	///push_back()
	template<typename T, class Allocator>
	void vector<T, Allocator>::push_back(const T &value)
	{
		this->reserve(_size + 1);
		alloc.construct(arr + _size, value);
		_size++;
	}

	template<typename T, class Allocator>
	void vector<T, Allocator>::pop_back()
	{
		_size--;
		alloc.destroy(arr + _size);
	}

	///resize()
	template<typename T, class Allocator>
	void vector<T, Allocator>::resize(vector::size_type count, T value)
	{
		if (_size > count){
			while (_size > count)
				alloc.destroy(arr + --_size);
			return;
		}
		this->reserve(count);
		while (_size < count)
			alloc.construct(arr + _size++);
	}

	///swap()
	template<typename T, class Allocator>
	void vector<T, Allocator>::swap(vector &other)
	{
		pointer temp_arr = other.arr;
		size_type temp_size = other._size;
		size_type temp_cap = other._capacity;
		Allocator temp_alloc = other.alloc;

		other.arr = arr;
		other._size = _size;
		other._capacity = _capacity;
		other.alloc = alloc;

		arr = temp_arr;
		_size = temp_size;
		_capacity = temp_cap;
		alloc = temp_alloc;
	}

	//Operators ---------------------------------------------------------------
	//todo?
}

#endif //FT_CONTAINERS_VECTOR_HPP
