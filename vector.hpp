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
		size_t size;
		T *	arr;
		Allocator alloc;

	public:
		vector();
		explicit vector(const Allocator & alloc);
		vector(const vector<T, Allocator> & inst);
		~vector();

		typedef T										value_type;
		typedef Allocator								alloc_type;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;
		//todo typedef iterators

		vector<T, Allocator> & operator=(const vector<T, Allocator> &rhs);
		T & operator[](size_t i);

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

	///copy = operator
	template<typename T, class Allocator>
	vector<T, Allocator> & vector<T, Allocator>::operator=(const vector<T, Allocator> &rhs)
	{
		T *temp = Allocator::allocate(rhs.size);

		std::memcpy(temp, rhs.arr, sizeof(T) * rhs.size);
		alloc.deallocate(arr, size);
		this->size = rhs.size;
		this->arr = temp;
		return *this;
	}

	///access [] operator
	template<typename T, class Allocator>
	T &vector<T, Allocator>::operator[](size_t i)
	{
		return arr[i];
	}
}

#endif //FT_CONTAINERS_VECTOR_HPP
