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

		void assign(size_type count, const T& value );
		template< class InputIt >
		void assign( InputIt first, InputIt last );
		allocator_type get_allocator() const;
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
	T &vector<T, Allocator>::operator[](size_t i)
	{
		return arr[i];
	}

	//Member functions
	///assign() with value
	template<typename T, class Allocator>
	void vector<T, Allocator>::assign(size_type count, const T& value){
		alloc.deallocate(arr, size);
		this->arr = Allocator::allocate(count);
		this->size = size;
		for (int i = 0; i < count; ++i)
		{
			this->arr[i] = value; //todo faster ??
		}
	}

	///assign() with list
	template<typename T, class Allocator>
	template< class InputIt >
	void vector<T, Allocator>::assign( InputIt first, InputIt last ){
		size_type count = last - first;//todo our ft::distance

		alloc.deallocate(arr, size);
		this->arr = Allocator::allocate(count);
		this->size = size;
		for (size_type i = 0; i < count; ++i)
		{
			this->arr[i] = first++; //todo faster ??
		}
	}

	///get_allocator()
	template<typename T, class Allocator>
	typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const{
		return alloc;
	}
}

#endif //FT_CONTAINERS_VECTOR_HPP
