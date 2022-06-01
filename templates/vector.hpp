//
// Created by Tadeusz Kondracki on 4/21/22.
//

#ifndef FT_CONTAINERS_VECTOR_HPP
# define FT_CONTAINERS_VECTOR_HPP

#include <cstddef>
#include <cstring>

namespace ft {

	///Vector Template
	//Class Declaration
	template<typename T, class Allocator = std::allocator<T> >
	class vector {
	private:
		size_t capacity;
		size_t size;
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
		//todo typedef iterators

		vector<T, Allocator> & operator=(const vector<T, Allocator> &rhs);
		T & operator[](size_t i);

		//funcs
		void assign(size_type count, const T& value );
		template< class InputIt >
		void assign( InputIt first, InputIt last );
		allocator_type get_allocator() const;
		//access
		reference at( size_type pos );
		const_reference at( size_type pos ) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;

		//capacity
		bool empty() const;
	};

	//Function Definitions
	//Constructors

	///default constructor
	template<typename T, class Allocator>
	vector<T, Allocator>::vector(): size(0)
	{
		arr = alloc.allocate(0);
	}

	///allocator constructor
	template<typename T, class Allocator>
	vector<T, Allocator>::vector(const Allocator &alloc): alloc(alloc), size(0)
	{
		arr = alloc.allocate(0);
	}

	///copy constructor
	template<typename T, class Allocator>
	vector<T, Allocator>::vector(const vector<T, Allocator> &inst) : size(inst.size)
	{
		arr = alloc.allocate(size);

		std::memcpy(arr, inst.arr, sizeof(T) * size);
	}

	///default destructor
	template<typename T, class Allocator>
	vector<T, Allocator>::~vector()
	{
		alloc.deallocate(arr, size);
	}

	//Operators
	///copy = operator
	template<typename T, class Allocator>
	vector<T, Allocator> & vector<T, Allocator>::operator=(const vector<T, Allocator> &rhs)
	{
		T *temp = alloc.allocate(rhs.size);

		std::memcpy(temp, rhs.arr, sizeof(T) * rhs.size);
		alloc.deallocate(arr, size);
		this->size = rhs.size;
		this->arr = temp;
		return *this;
	}

	///access [] operator
	template<typename T, class Allocator>
	T &vector<T, Allocator>::operator[](vector::size_type i)
	{
		return arr[i];
	}

	//Member functions
	///assign() with value
	template<typename T, class Allocator>
	void vector<T, Allocator>::assign(vector::size_type count, const T& value){ //todo resize
		alloc.deallocate(arr, size);
		this->arr = Allocator::allocate(count, arr);
		this->size = size;
		for (int i = 0; i < count; ++i)
		{
			this->arr[i] = value;
		}
	}

	///assign() with list
	template<typename T, class Allocator>
	template< class InputIt >
	void vector<T, Allocator>::assign( InputIt first, InputIt last ){
		size_type count = last - first;

		alloc.deallocate(arr, size);
		this->arr = Allocator::allocate(count, arr);
		this->size = size;
		for (size_type i = 0; i < count; ++i)
		{
			this->arr[i] = first++;
		}
	}

	///get_allocator()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const{
		return alloc;
	}

	///at()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(vector::size_type pos)
	{
		if (pos >= size)
			throw std::out_of_range("vector out of bounds access");
		return arr[pos];
	}

	///at() const
	template<typename T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(vector::size_type pos) const
	{
		if (pos >= size)
			throw std::out_of_range("vector out of bounds access");
		return arr[pos];
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
		return arr[size - 1];
	}

	///back() const
	template<typename T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const
	{
		return arr[size - 1];
	}

	//todo data resume

	//Capacity
	///empty()
	template<typename T, class Allocator>
	bool vector<T, Allocator>::empty() const {
		if (size == 0)
			return true;
		return false;
	}
}

#endif //FT_CONTAINERS_VECTOR_HPP
