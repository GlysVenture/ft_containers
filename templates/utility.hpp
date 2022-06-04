//
// Created by tkondrac on 6/3/22.
//

#ifndef FT_CONTAINERS_UTILITY_HPP
#define FT_CONTAINERS_UTILITY_HPP

#include <cstdlib>
#include <memory>

namespace ft {

	///useful kind of std::copy but no really copy func with construction and destruction
	template<typename T, class Allocator>
	void copy(T * dest, T * src, size_t n, Allocator alloc) {
		for (size_t i = 0; i < n; ++i){
			alloc.construct(dest + i, src[i]);
		}
	}

	///rebuilds in different spot
	template<typename T, class Allocator>
	void rebuild(T * dest, T * src, size_t n, Allocator alloc) {
		for (size_t i = 0; i < n; ++i){
			alloc.construct(dest + i, src[i]);
			alloc.destroy(src + i);
		}
	}

	///moves arr safely
	template<typename T, class Allocator>
	void move(T * dest, T * src, size_t n, Allocator alloc) {
		T * temp = alloc.allocate(n);
		for (size_t i = 0; i < n; ++i){
			alloc.construct(temp + i, src[i]);
			alloc.destroy(src + i);
		}
		for (size_t i = 0; i < n; ++i){
			alloc.construct(dest + i, temp[i]);
			alloc.destroy(temp + i);
		}
		alloc.deallocate(temp, n);
	}

	///ft::pair template
	template<class T1, class T2>
	struct pair {
		T1	first;
		T2	second;

		typedef T1	first_type;
		typedef T2	second_type;

		pair() {  }
		pair( const T1& x, const T2& y ): first(x), second(y) { }
		pair( const pair<T1, T2>& p ): first(p.first), second(p.second) {  }
		~pair() { }

		pair & operator=(const pair & p) {
			first = p.first;
			second = p.second;
			return *this;
		}
	};

	//Pair operators <=>

	template< class T1, class T2 >
	bool operator==( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs ) {
		if (lhs.first == rhs.first && lhs.second == rhs.second)
			return true;
		return false;
	}

	template< class T1, class T2 >
	bool operator!=( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs ) {
		if (lhs.first != rhs.first || lhs.second != rhs.second)
			return true;
		return false;
	}

	template< class T1, class T2 >
	bool operator<( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs ) {
		if (lhs.first < rhs.first && lhs.second < rhs.second)
			return true;
		return false;
	}

	template< class T1, class T2 >
	bool operator>( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs ) {
		if (lhs.first > rhs.first && lhs.second > rhs.second)
			return true;
		return false;
	}

	template< class T1, class T2 >
	bool operator<=( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs ) {
		if (lhs.first <= rhs.first && lhs.second <= rhs.second)
			return true;
		return false;
	}

	template< class T1, class T2 >
	bool operator>=( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs ) {
		if (lhs.first >= rhs.first && lhs.second >= rhs.second)
			return true;
		return false;
	}

}

#endif //FT_CONTAINERS_UTILITY_HPP
